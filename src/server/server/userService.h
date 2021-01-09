#pragma once
#include "abstractUserService.h"
#include "service.h"
#include <unordered_map>

class UserService : public AbstractUserService, public Service {
public:
	UserService(AbstractServiceProvider* provider);
	virtual void init();
	virtual void saveUserPosition(User user);
	virtual void login(std::string username, std::function<void(std::shared_ptr<User>)> onSuccess);
	virtual void logout(User user);
	virtual std::shared_ptr<User> getUserById(int id);
};
