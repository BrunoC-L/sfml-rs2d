#pragma once
#include <vector>
#include <functional>
#include "../../common/abstractService.h"
#include "user.h"

class AbstractUserService : public AbstractService {
public:
	std::vector<std::shared_ptr<User>> users;
	AbstractUserService() { }
	virtual void saveUserPosition(User user) = 0;
	virtual void login(std::string username, std::function<void(std::shared_ptr<User>)> onSuccess) = 0;
	virtual void logout(User user) = 0;
	virtual std::shared_ptr<User> getUserById(int id) = 0;
};
