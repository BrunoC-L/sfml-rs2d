#pragma once
#include <vector>
#include <functional>
#include "json.h"
#include "abstractService.h"

using QueryResult = std::vector<JSON>;
using Query = std::pair<std::string, std::function<void(QueryResult)>>;

class AbstractDB : public AbstractService {
protected:
public:
	virtual void query(std::string, std::function<void(QueryResult)> = [](QueryResult) {}) = 0;
	virtual void queryPlayerByUsernameEquals(std::string username, std::function<void(QueryResult)>) = 0;
	virtual void queryLoginDataByUserId(int id, std::function<void(QueryResult)> f) = 0;
	virtual QueryResult syncQuery(std::string s, bool warn = true) = 0;
};
