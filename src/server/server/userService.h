#pragma once
#include "abstractUserService.h"
#include "service.h"
#include <unordered_map>

class UserService : public AbstractUserService, public Service {
private:
	std::map<std::shared_ptr<User>, std::string> tempSaltByUser;
public:
	UserService(ServiceProvider* provider);
	virtual void init();
	virtual void saveUserPosition(User& user);
	virtual void logout(User& user);
	virtual std::shared_ptr<User> getUserById(int id);
};
