#include "db.h"
#include "odbc.h"
#include "onExit.h"
#include "print.h"
#include "logger.h"
#include "session.h"
#include <stdlib.h>
#include <string>
#include <codecvt>
#include <locale>

#ifdef __APPLE__
#define _ASSERT assert
#endif // __APPLE__

DB::DB(ServiceProvider* provider, int nThreads) : Service(provider), nThreads(nThreads) {
	provider->set(DATABASE, this);
}

void DB::init() {
	defaultFolderLogger(getSession().get("logs").get("server").asString(), "db.txt", true)("db init\n");
	acquire();
	dbThreadPool.reserve(nThreads);
	connect(1);
	std::cout << "Connected to DB, Performing DB checks for version " + version << std::endl;
	if (isEmpty())
		createDB();
	else
		checkVersion();
	std::cout << "Database is up to date\n";
	connect(nThreads - 1);
	logoutObserver.set([&](LogoutEvent& ev) {
		ids[ev.user->index] = -1;
	});
	loginObserver.set([&](LoginEvent& ev) {
		auto qr = syncSelectQuery("select id from player where username = '" + ev.user->ign + "'");
		_ASSERT(qr.size() == 1);
		auto id = qr[0]["id"].asInt();
		ids[ev.user->index] = id;
	});
	playerMoveEventObserver.set([&](PlayerMoveEvent& ev) {
		saveUserPosition(*ev.user, ev.position);
	});
}

void DB::connect(int n) {
	auto session = getSession();
	auto dbcstr = session["dbconnectionstring"].asString("", true);
	// https://riptutorial.com/cplusplus/example/4190/conversion-to-std--wstring
	auto ws = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(dbcstr);
	const WCHAR* pwszConnStr = ws.c_str();
	for (int i = 0; i < n; ++i) {
		dbThreadPool.push_back({});
		auto& dbthread = dbThreadPool.back();
		std::shared_ptr<bool> failed = std::make_shared<bool>(false);
		dbthread.second = false;
		auto log = defaultFolderLogger(session.get("logs").get("server").asString(), "db.txt", true);
		dbthread.first = std::thread(
			[&, failed, log]() {
				OnExit e([&, failed]() {
					if (failed != nullptr)
						*failed = true;
				});
				{
					std::ostringstream ss;
					ss << "DB Thread: " << std::this_thread::get_id() << std::endl;
					print(ss);
					log(ss.str());
				}
				db(pwszConnStr, queryLock, sQueries, nsQueries, waiter, cv, &dbthread.second, "create database " + dbname + "; use " + dbname + "; SET IMPLICIT_TRANSACTIONS ON;", threadPoolInitMutex);
				{
					std::ostringstream ss;
					ss << "DB Thread " << std::this_thread::get_id() << " Exiting" << std::endl;
					print(ss);
					log(ss.str());
				}
			}
		);

		while (!dbthread.second)
			if (*failed)
				throw std::runtime_error("DB failed to start\n");
	}
}

bool DB::isEmpty() {
	auto q = syncSelectQuery("select name from sysobjects where xtype = 'U';");
	auto nobjects = q.size();
	return nobjects == 0;
}

void DB::createDB() {
	syncNonSelectQuery("create table version(version varchar(25) not null);");
	syncNonSelectQuery("insert into version values('1');");
	syncNonSelectQuery("create table player(id int not null identity, username varchar(25) not null unique, posx int default 1172, posy int default 869, primary key (id));");
	syncNonSelectQuery("create table logindata(id int not null, salt varchar(64) not null, hash varchar(64) not null, primary key(id), foreign key(id) references player(id));");
}

void DB::checkVersion() {
	bool versionTableMissing = syncSelectQuery("select * from sysobjects where xtype = 'U' and name = 'version';").size() == 0;
	if (versionTableMissing)
		throw std::runtime_error("Found tables in database but missing version table");
	auto versions = syncSelectQuery("select version from version;");
	auto version = versions[0]["version"].asString();
	if (version != this->version)
		updateVersion(version);
}

void DB::updateVersion(std::string version) {
	throw std::runtime_error("Not implemented exception");
}

void DB::selectQuery(std::string s, std::function<void(SelectQueryResult)> f) {
	{
		std::lock_guard<std::mutex> lock(queryLock);
		sQueries.push_back(std::make_pair(s + "; commit;", f));
	}
	cv.notify_one();
}

void DB::nonSelectQuery(std::string s, std::function<void(NonSelectQueryResult)> f) {
	{
		std::lock_guard<std::mutex> lock(queryLock);
		nsQueries.push_back(std::make_pair(s + "; commit;", f));
	}
	cv.notify_one();
}

SelectQueryResult DB::syncSelectQuery(std::string s) {
	bool ready = false, completed = false;
	SelectQueryResult result;
	std::mutex m;
	std::condition_variable cv;
	selectQuery(s, [&](SelectQueryResult qr) {
		while (!ready);
		OnExit e([&]() {
			cv.notify_one();
			completed = true;
		});
		result = qr;
	});
	std::unique_lock<std::mutex> u(m);
	cv.wait(u, [&]() {
		ready = true;
		return completed;
	});
	return result;
}

NonSelectQueryResult DB::syncNonSelectQuery(std::string s) {
	bool ready = false, completed = false;
	NonSelectQueryResult result;
	std::mutex m;
	std::condition_variable cv;
	nonSelectQuery(s, [&](NonSelectQueryResult qr) {
		while (!ready);
		OnExit e([&]() {
			cv.notify_one();
			completed = true;
		});
		result = qr;
	});
	std::unique_lock<std::mutex> u(m);
	cv.wait(u, [&]() {
		ready = true;
		return completed;
	});
	return result;
}

void DB::queryPlayerByUsernameEquals(std::string username, std::function<void(SelectQueryResult)> f) {
	std::string s = "select * from Player where username = '" + username + "'\n";
	selectQuery(s, f);
}

void DB::queryLoginDataByUserId(int id, std::function<void(SelectQueryResult)> f) {
	std::string s = "select * from LoginData where id = '" + std::to_string(id) + "'\n";
	selectQuery(s, f);
}

void DB::stop() {
	for (auto& dbthread : dbThreadPool)
		dbthread.second = false;
	cv.notify_all();
	for (auto& dbthread : dbThreadPool)
		dbthread.first.join();
}

void DB::saveUserPosition(const User& user, VTile position) {
	std::string q = "update Player set posx = " +
		std::to_string(position.x) +
		", posy = " +
		std::to_string(position.y) +
		" where id = '" + std::to_string(ids[user.index]) + "'\n";
	nonSelectQuery(q);
}
