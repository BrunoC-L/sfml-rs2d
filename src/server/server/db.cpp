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
    err = wcscmp(L"DRIVER=SQL Server Native Client 11.0;SERVER=DESKTOP-FJJ4HB5\\SQLEXPRESS;DATABASE=rs2d;Trusted_Connection=Yes;", pwszConnStr);
    if (err)
        throw 1;
	dbthread = std::thread(
		[&]() {
			db(pwszConnStr, queries, mutex, &connected);
		}
	);
	while (!connected);


    query("select * from sysobjects where xtype = 'U';", [&](QueryResult qr) {
        bool DBisEmpty = qr.size() == 0;
        if (DBisEmpty)
            createDB();
        else {
            query("select * from sysobjects where xtype = 'U' and name = 'version';", [&](QueryResult qr) {
                bool versionTableMissing = qr.size() == 0;
                if (versionTableMissing)
                    throw std::exception("Found tables in database but missing version table");
                query("select * from version;", [&](QueryResult qr) {
                    bool versionMissing = qr.size() == 0;
                    if (versionMissing)
                        throw std::exception("Found version table in db but its empty");
                    std::string version = qr[0][0];
                    std::cout << "Performing DB checks for version " + version << std::endl;
                    checkVersion(version);
                    if (version != this->version) {
                        std::cout << "Performing DB updates from version " + version + " to version " + this->version << std::endl;
                        updateVersion(version);
                    }
                });
            });
        }
    });
}

void DB::createDB() {
    query("create table player(id int, name varchar(20), username varchar(20), password varchar(20), posx int, posy int);");
}

void DB::checkVersion(std::string version) {
    // for each expected table
    // for each expected column
    // check type
}

void DB::updateVersion(std::string version) {

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
