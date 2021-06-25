#pragma once
#include "service.h"
#include <unordered_map>
#include "logoutEvent.h"
#include "constants.h"
#include "gameMessageEvent.h"
#include <vector>
#include <functional>
#include "user.h"
#include <memory>

class UserService : public Service {
private:
	std::unordered_map<std::shared_ptr<User>, std::string> tempSaltByUser;
	EventObserver<LogoutEvent> logoutObserver;
	std::shared_ptr<User> users[MAX_PLAYERS_ONLINE];
	std::vector<int> availableIndices;
	std::vector<std::shared_ptr<User>> iteratableUsers;
	virtual void logout(const std::shared_ptr<User>& user);
	EventObserver<GameMessageEvent> gameMessageObserver;
public:
	UserService(ServiceProvider* provider);
	virtual void init();
	virtual const std::shared_ptr<User>& getUserByIndex(int index);
	virtual const std::vector<std::shared_ptr<User>>& getAllUsers();
};
