#pragma once
#include "abstractUserService.h"
#include "service.h"
#include <unordered_map>
#include "loginout.h"

class UserService : public AbstractUserService, public Service {
private:
	std::map<std::shared_ptr<User>, std::string> tempSaltByUser;
	EventObserver<LogoutEvent> logoutObserver;
public:
	UserService(ServiceProvider* provider);
	virtual void init() override;
	virtual void saveUserPosition(User& user) override;
	virtual void logout(User& user) override;
	virtual std::shared_ptr<User> getUserById(int id) override;
	virtual void stop() override;
};
