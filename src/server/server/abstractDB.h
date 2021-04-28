#pragma once
#include <vector>
#include <functional>
#include "json.h"
#include "abstractService.h"

using SelectQueryResult = std::vector<JSON>;
using NonSelectQueryResult = std::string;

class AbstractDB : public AbstractService {
protected:
public:
	virtual void nonSelectQuery(std::string, std::function<void(NonSelectQueryResult)> = [](NonSelectQueryResult) {}) = 0;
	virtual NonSelectQueryResult syncNonSelectQuery(std::string s, bool warn = true) = 0;
	
	virtual void selectQuery(std::string, std::function<void(SelectQueryResult)>) = 0;
	[[nodiscard]]  virtual SelectQueryResult syncSelectQuery(std::string s, bool warn = true) = 0;

	virtual void queryPlayerByUsernameEquals(std::string username, std::function<void(SelectQueryResult)>) = 0;
	virtual void queryLoginDataByUserId(int id, std::function<void(SelectQueryResult)> f) = 0;

	virtual void stop() = 0;
};
