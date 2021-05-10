#pragma once
#include <vector>
#include <functional>
#include "abstractService.h"
#include "user.h"
#include <memory>

class AbstractUserService : public AbstractService {
public:
	AbstractUserService() { }
	virtual void stop() = 0;
	virtual const std::shared_ptr<User>& getUserByIndex(int index) = 0;
	virtual const std::vector<std::shared_ptr<User>> getAllUsers() = 0;
};
