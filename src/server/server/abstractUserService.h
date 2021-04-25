#pragma once
#include <vector>
#include <functional>
#include "abstractService.h"
#include "user.h"

class AbstractUserService : public AbstractService {
public:
	std::vector<std::shared_ptr<User>> users;
	AbstractUserService() { }
	virtual void saveUserPosition(User& user) = 0;
	virtual void logout(User& user) = 0;
	virtual std::shared_ptr<User> getUserById(int id) = 0;
};
