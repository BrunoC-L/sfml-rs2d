#pragma once
#include <mutex>
#include "loginEvent.h"
#include "logoutEvent.h"
#include "constants.h"
#include "playerMoveEvent.h"
#include <vector>
#include <functional>
#include "json.h"
#include "service.h"

using SelectQueryResult = std::vector<JSON>;
using NonSelectQueryResult = std::string;

using SelectQuery = std::pair<std::string, std::function<void(SelectQueryResult)>>;
using NonSelectQuery = std::pair<std::string, std::function<void(NonSelectQueryResult)>>;

class DB : public Service {
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

	EventObserver<LoginEvent> loginObserver;
	EventObserver<LogoutEvent> logoutObserver;
	EventObserver<PlayerMoveEvent> playerMoveEventObserver;

	void createDB();
	void checkVersion();
	void updateVersion(std::string version);
	void connect(int n);
	bool isEmpty();
	void saveUserPosition(const User& user, VTile position);
public:
	int ids[MAX_PLAYERS_ONLINE]{ 0 };

	DB(ServiceProvider* provider, int nThreads);
	~DB() {
		throw 1;
	}
	virtual void init();
	virtual void nonSelectQuery(std::string, std::function<void(NonSelectQueryResult)> = [](NonSelectQueryResult) {});
	virtual NonSelectQueryResult syncNonSelectQuery(std::string s);

	virtual void selectQuery(std::string, std::function<void(SelectQueryResult)>);
	virtual SelectQueryResult syncSelectQuery(std::string s);

	virtual void queryPlayerByUsernameEquals(std::string username, std::function<void(SelectQueryResult)>);
	virtual void queryLoginDataByUserId(int id, std::function<void(SelectQueryResult)> f);

	virtual void stop();
};
