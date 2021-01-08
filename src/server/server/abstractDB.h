#pragma once
#include <vector>
#include <functional>
#include "../../common/abstractService.h"

using QueryResult = std::vector<std::vector<std::string>>;
using Query = std::pair<std::string, std::function<void(QueryResult)>>;

class AbstractDB : public AbstractService {
protected:
	wchar_t* connectionString;
public:
	AbstractDB(wchar_t* connectionString) : connectionString(connectionString) { }
	virtual void query(Query) = 0;
	virtual void query(std::string) = 0;
	virtual void query(std::string, std::function<void(QueryResult)>) = 0;
	virtual void queryPlayerByUsernameEquals(std::string username, std::function<void(QueryResult)>) = 0;
};
