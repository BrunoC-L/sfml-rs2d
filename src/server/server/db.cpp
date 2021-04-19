#include "db.h"
#include "odbc.h"

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
			db(pwszConnStr, queries, mutex, &connected);
		}
	);
	while (!connected);
}

bool DB::isEmpty() {
	return syncQuery("select name from sysobjects where xtype = 'U';", false).size() == 0;
}

void DB::sanitizeArgumentAlphaNumericSpace(std::string& arg) {
	for (auto& c : arg) {
		if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == ' '))
			continue;
		c = ' ';
	}
}

void DB::createDB() {
	query("create table player(id int not null identity, name varchar(25) not null, username varchar(25) not null unique, password varchar(25) not null, posx int not null, posy int not null, primary key (id));");
	query("create table version(version varchar(25) not null);");
	query("insert into version values('1');");
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
	queries.push_back(std::make_pair(s, f));
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
	sanitizeArgumentAlphaNumericSpace(username);
	std::string s = "select * from Player where username = '" + username + "'\n";
	query(s, f);
}
