#pragma once
#include "service.h"
#include "abstractDB.h"
#include <mutex>

class DB : public Service, public AbstractDB {
	std::mutex mutex;
	std::vector<Query> queries;
	std::thread dbthread;
public:
	DB(AbstractServiceProvider* provider, wchar_t* connectionString);
	virtual void init();
	virtual void query(Query);
	virtual void query(std::string);
	virtual void query(std::string, std::function<void(QueryResult)>);
	virtual void queryPlayerByUsernameEquals(std::string username, std::function<void(QueryResult)>);
};