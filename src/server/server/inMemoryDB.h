#pragma once
#include "abstractDB.h"
#include "serviceProvider.h"
#include "service.h"

//struct InMemoryDBPlayer {
//    std::string username;
//    std::string id;
//    std::string posX, posY;
//};

//struct InMemoryDBLoginData {
//    std::string id;
//    std::string salt;
//    std::string hash;
//};

class InMemoryDB : public Service, public AbstractDB {
public:
	InMemoryDB(ServiceProvider* provider);
	virtual void stop() override;
	virtual void init() override;

	virtual void queryPlayerByUsernameEquals(std::string username, std::function<void(SelectQueryResult)>) override;
	virtual void queryLoginDataByUserId(int id, std::function<void(SelectQueryResult)> f) override;
	virtual void selectIdFromPlayerWhereUsernameEquals(std::string username, std::function<void(SelectQueryResult)> f) override;
	virtual void createPlayerWithUsername(std::string username) override;
	virtual void insertLoginDataIdPermSaltpwHashPermSalt(std::string id, std::string permSalt, std::string hash) override;
	virtual void selectLogindataWithUsername(std::string username, std::function<void(SelectQueryResult)> f) override;

protected:
    std::vector<JSON> playerTable;
    std::vector<JSON> loginDataTable;
    
};
