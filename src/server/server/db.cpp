#include "db.h"
#include "odbc.h"

DB::DB(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("DB", this);
}

void DB::init() {
	acquire();
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

void DB::query(Query q) {
	queries.push_back(q);
}

void DB::query(std::string s) {
	queries.push_back(std::make_pair(s, [](QueryResult) {}));
}

void DB::query(std::string s, std::function<void(QueryResult)> f) {
	queries.push_back(std::make_pair(s, f));

}

void DB::queryPlayerByUsernameEquals(std::string username, std::function<void(QueryResult)> f) {
	std::string s = "select * from Player where username = '" + username + "'\n";
	queries.push_back(std::make_pair(s, f));
}
