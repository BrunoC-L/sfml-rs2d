#include "db.h"
#include "odbc.h"
#include "queryBuilder.h"

DB::DB(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("DB", this);
}

void DB::init() {
	acquire();
	connect();
	std::cout << "Connected to DB, Performing DB checks for version " + version << std::endl;
	if (isEmpty())
		createDB();
	else
		checkVersion();
	std::cout << "Database is up to date\n";
}

void DB::connect() {
	FILE* pFile;
	const int s = 1000;
	wchar_t pwszConnStr[s];
	char* buf = nullptr;
	size_t sz = 0;
	std::string path;
	std::string fileName = "dbinfo.txt";
	if (_dupenv_s(&buf, &sz, "ODBC_RS2D_HOME") == 0 && buf == nullptr)
		throw std::exception("No environment variable set for 'ODBC_RS2D_HOME'");
	path = std::string(buf);
	free(buf);
	auto err = fopen_s(&pFile, (path + "/" + fileName).c_str(), "r");
	if (pFile == NULL || err != 0)
		throw std::exception((fileName + " not found at 'ODBC_RS2D_HOME'").c_str());
	fgetws(pwszConnStr, s, pFile);
	fclose(pFile);
	dbthread = std::thread(
		[&]() {
			db(pwszConnStr, queryLock, queries, waiter, cv, &connected);
		}
	);
	while (!connected);
}

bool DB::isEmpty() {
	return syncQuery("select name from sysobjects where xtype = 'U';", false).size() == 0;
}

void DB::createDB() {
	query("create table version(version varchar(25) not null);");
	query("insert into version values('1');");

	query("create table player(id int not null identity, username varchar(25) not null unique, posx int default 1172, posy int default 869, primary key (id));");
	query("create table logindata(id int not null, salt varchar(64) not null, hash varchar(64) not null, primary key(id), foreign key(id) references player(id));");
}

void DB::checkVersion() {
	bool versionTableMissing = syncQuery("select * from sysobjects where xtype = 'U' and name = 'version';").size() == 0;
	if (versionTableMissing)
		throw std::exception("Found tables in database but missing version table");
	auto versions = syncQuery("select version from version;");
	auto version = versions[0]["version"].asString();
	if (version != this->version)
		updateVersion(version);
}

void DB::updateVersion(std::string version) {
	throw std::exception("Not implemented exception");
}

void DB::query(std::string s, std::function<void(QueryResult)> f) {
	queryLock.lock();
	queries.push_back(std::make_pair(s, f));
	queryLock.unlock();
	cv.notify_one();
}

QueryResult DB::syncQuery(std::string s, bool warn) {
	if (warn)
		std::cout << "Synchronous DB operation on thread " << std::this_thread::get_id() << std::endl;
	bool completed = false;
	QueryResult result;
	query(s, [&](QueryResult qr) {
		result = qr;
		completed = true;
	});
	while (!completed);
	return result;
}

void DB::queryPlayerByUsernameEquals(std::string username, std::function<void(QueryResult)> f) {
	std::string s = "select * from Player where username = '" + username + "'\n";
	query(s, f);
}

void DB::queryLoginDataByUserId(int id, std::function<void(QueryResult)> f) {
	std::string s = "select * from LoginData where id = '" + std::to_string(id) + "'\n";
	query(s, f);
}
