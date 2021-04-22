#pragma once
#include "service.h"
#include "abstractDB.h"
#include <mutex>

class DB : public Service, public AbstractDB {
	std::mutex queryLock;
	std::vector<Query> queries;

	std::mutex waiter;
	std::condition_variable cv;

	std::thread dbthread;
	bool connected = false;
	void createDB();
	void checkVersion();
	void updateVersion(std::string version);
	void connect();
	bool isEmpty();
public:
	DB(AbstractServiceProvider* provider);
	virtual void init();
	virtual void query(std::string, std::function<void(QueryResult)> = [](QueryResult) {});
	virtual QueryResult syncQuery(std::string s, bool warn = true);
	virtual void queryPlayerByUsernameEquals(std::string username, std::function<void(QueryResult)>);
	virtual void queryLoginDataByUserId(int id, std::function<void(QueryResult)> f) override;
	std::string version = "1";
};
