#include "db.h"
#include "odbc.h"
#include "onExit.h"
#include "print.h"
#include "getenv.h"

DB::DB(ServiceProvider* provider) : Service(provider) {
	provider->set("DB", this);
}

void DB::init() {
	acquire();
	connect();
	std::cout << "Connected to DB, Performing DB checks for version " + version << std::endl;
	syncNonSelectQuery("create database rs2d;");
	syncNonSelectQuery("use rs2d;");
	if (isEmpty())
		createDB();
	else
		checkVersion();
	std::cout << "Database is up to date\n";
}

void DB::connect() {
	std::string env = "RS2D_HOME";
	std::string fileName = "dbinfo.txt";

	std::string path = getenv(env);

	FILE* pFile;
	auto err = fopen_s(&pFile, (path + "/" + fileName).c_str(), "r");
	if (pFile == NULL || err != 0)
		throw std::exception((fileName + " not found at '" + env + "'").c_str());

	const int s = 1000;
	wchar_t pwszConnStr[s];
	fgetws(pwszConnStr, s, pFile);
	fclose(pFile);

	std::shared_ptr<bool> failed = std::make_shared<bool>(false);

	dbthread = std::thread(
		[&, failed]() {
			OnExit e([&, failed]() {
				if (failed != nullptr)
					*failed = true;
			});
			{
				std::ostringstream ss;
				ss << "DB Thread: " << std::this_thread::get_id() << std::endl;
				print(ss);
			}
			db(pwszConnStr, queryLock, sQueries, nsQueries, waiter, cv, &connected);
			{
				std::ostringstream ss;
				ss << "DB Thread " << std::this_thread::get_id() << " Exiting" << std::endl;
				print(ss);
			}
		}
	);

	while (!connected) {
		if (*failed)
			throw std::exception("DB failed to start\n");
	}
}

bool DB::isEmpty() {
	auto q = syncSelectQuery("select name from sysobjects where xtype = 'U';", false);
	auto nobjects = q.size();
	return nobjects == 0;
}

void DB::createDB() {
	nonSelectQuery("create table version(version varchar(25) not null);");
	nonSelectQuery("insert into version values('1');");
	nonSelectQuery("create table player(id int not null identity, username varchar(25) not null unique, posx int default 1172, posy int default 869, primary key (id));");
	nonSelectQuery("create table logindata(id int not null, salt varchar(64) not null, hash varchar(64) not null, primary key(id), foreign key(id) references player(id));");
}

void DB::checkVersion() {
	bool versionTableMissing = syncSelectQuery("select * from sysobjects where xtype = 'U' and name = 'version';").size() == 0;
	if (versionTableMissing)
		throw std::exception("Found tables in database but missing version table");
	auto versions = syncSelectQuery("select version from version;");
	auto version = versions[0]["version"].asString();
	if (version != this->version)
		updateVersion(version);
}

void DB::updateVersion(std::string version) {
	throw std::exception("Not implemented exception");
}

void DB::selectQuery(std::string s, std::function<void(SelectQueryResult)> f) {
	{
		std::lock_guard<std::mutex> lock(queryLock);
		sQueries.push_back(std::make_pair(s, f));
	}
	cv.notify_one();
}

void DB::nonSelectQuery(std::string s, std::function<void(NonSelectQueryResult)> f) {
	{
		std::lock_guard<std::mutex> lock(queryLock);
		nsQueries.push_back(std::make_pair(s, f));
	}
	cv.notify_one();
}

SelectQueryResult DB::syncSelectQuery(std::string s, bool warn) {
	if (warn)
		std::cout << "Synchronous DB operation on thread " << std::this_thread::get_id() << std::endl;
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

NonSelectQueryResult DB::syncNonSelectQuery(std::string s, bool warn) {
	if (warn)
		std::cout << "Synchronous DB operation on thread " << std::this_thread::get_id() << std::endl;
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
	connected = false;
	cv.notify_one();
	dbthread.join();
}
