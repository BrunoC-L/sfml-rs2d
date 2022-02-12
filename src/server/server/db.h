#pragma once
#include "service.h"
#include "abstractDB.h"
#include <mutex>
#include "loginEvent.h"
#include "logoutEvent.h"
#include "constants.h"
#include "playerMoveEvent.h"

using SelectQuery = std::pair<std::string, std::function<void(SelectQueryResult)>>;
using NonSelectQuery = std::pair<std::string, std::function<void(NonSelectQueryResult)>>;

class DB : public Service, public AbstractDB {
protected:
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
	int ids[MAX_PLAYERS_ONLINE];

	EventObserver<LoginEvent> loginObserver;
	EventObserver<LogoutEvent> logoutObserver;
	EventObserver<PlayerMoveEvent> playerMoveEventObserver;

	void createDB();
	void checkVersion();
	void updateVersion(std::string version);
	void connect(int n);
	bool isEmpty();
	void saveUserPosition(const User& user, VTile position);

	virtual void nonSelectQuery(std::string, std::function<void(NonSelectQueryResult)> = [](NonSelectQueryResult) {});
	virtual NonSelectQueryResult syncNonSelectQuery(std::string s);

	virtual void selectQuery(std::string, std::function<void(SelectQueryResult)>);
	virtual SelectQueryResult syncSelectQuery(std::string s);
public:

	DB(ServiceProvider* provider, int nThreads);
	virtual void init();

	virtual void queryPlayerByUsernameEquals(std::string username, std::function<void(SelectQueryResult)>) override;
	virtual void queryLoginDataByUserId(int id, std::function<void(SelectQueryResult)> f) override;
	virtual void selectIdFromPlayerWhereUsernameEquals(std::string username, std::function<void(SelectQueryResult)> f) override;
	virtual void createPlayerWithUsername(std::string username) override;
	virtual void insertLoginDataIdPermSaltpwHashPermSalt(std::string id, std::string permSalt, std::string hash) override;
	virtual void selectLogindataWithUsername(std::string username, std::function<void(SelectQueryResult)> f) override;

	virtual void stop() override;
};
