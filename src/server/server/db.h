#pragma once
#include "service.h"
#include "abstractDB.h"
#include <mutex>

using SelectQuery = std::pair<std::string, std::function<void(SelectQueryResult)>>;
using NonSelectQuery = std::pair<std::string, std::function<void(NonSelectQueryResult)>>;

class DB : public Service, public AbstractDB {
	std::mutex queryLock;
	std::vector<SelectQuery> sQueries;
	std::vector<NonSelectQuery> nsQueries;

	std::mutex waiter;
	std::condition_variable cv;

	std::mutex threadPoolInitMutex;
	std::vector<std::pair<std::thread, bool>> dbThreadPool;
	int nThreads;

	std::string dbname = "rs2d";

	std::string version = "1";

	void createDB();
	void checkVersion();
	void updateVersion(std::string version);
	void connect(int n);
	bool isEmpty();
public:
	DB(ServiceProvider* provider, int nThreads);
	virtual void init();
	virtual void nonSelectQuery(std::string, std::function<void(NonSelectQueryResult)> = [](NonSelectQueryResult) {}) override;
	virtual NonSelectQueryResult syncNonSelectQuery(std::string s) override;

	virtual void selectQuery(std::string, std::function<void(SelectQueryResult)>) override;
	virtual SelectQueryResult syncSelectQuery(std::string s) override;

	virtual void queryPlayerByUsernameEquals(std::string username, std::function<void(SelectQueryResult)>) override;
	virtual void queryLoginDataByUserId(int id, std::function<void(SelectQueryResult)> f) override;

	virtual void stop() override;
};
