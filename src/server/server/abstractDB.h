#pragma once
#include <vector>
#include <functional>
#include "json.h"
#include "abstractService.h"

using SelectQueryResult = std::vector<JSON>;
using NonSelectQueryResult = std::string;

class AbstractDB : public AbstractService {
public:
	virtual void queryPlayerByUsernameEquals(std::string username, std::function<void(SelectQueryResult)>) = 0;
	virtual void queryLoginDataByUserId(int id, std::function<void(SelectQueryResult)> f) = 0;
	virtual void selectIdFromPlayerWhereUsernameEquals(std::string username, std::function<void(SelectQueryResult)> f) = 0;
	virtual void createPlayerWithUsername(std::string username) = 0;
	virtual void insertLoginDataIdPermSaltpwHashPermSalt(std::string id, std::string permSalt, std::string hash) = 0;
	virtual void selectLogindataWithUsername(std::string username, std::function<void(SelectQueryResult)> f) = 0;

	virtual void stop() = 0;
};
