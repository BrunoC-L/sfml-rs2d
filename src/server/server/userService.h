#pragma once
#include "abstractUserService.h"
#include "service.h"
#include <unordered_map>
#include "logoutEvent.h"
#include "constants.h"

class UserService : public AbstractUserService, public Service {
private:
	std::map<std::shared_ptr<User>, std::string> tempSaltByUser;
	EventObserver<LogoutEvent> logoutObserver;
	std::shared_ptr<User> users[MAX_PLAYERS_ONLINE];
	std::vector<int> availableIndices;
	std::vector<std::shared_ptr<User>> iteratableUsers;
	virtual void logout(const std::shared_ptr<User>& user);
public:
	UserService(ServiceProvider* provider);
	virtual void init() override;
	virtual void stop() override;
	virtual const std::shared_ptr<User>& getUserByIndex(int index) override;
	virtual const std::vector<std::shared_ptr<User>>& getAllUsers() override;
};
