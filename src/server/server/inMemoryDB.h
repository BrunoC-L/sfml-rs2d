#pragma once
#include "abstractDB.h"
#include "serviceProvider.h"
#include "service.h"

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
};
