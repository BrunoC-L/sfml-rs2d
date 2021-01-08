#include "db.h"
#include "odbc.h"

DB::DB(AbstractServiceProvider* provider, wchar_t* connectionString) : Service(provider), AbstractDB(connectionString) {
	provider->set("DB", this);
}

void DB::init() {
	acquire();
	dbthread = std::thread(
		[&]() {
			db(connectionString, queries, mutex);
		}
	);
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
