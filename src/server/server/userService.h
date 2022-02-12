#pragma once
#include "abstractUserService.h"
#include "service.h"
#include <unordered_map>
#include "logoutEvent.h"
#include "constants.h"
#include "gameMessageEvent.h"
#include <mutex>

class UserService : public AbstractUserService, public Service {
protected:
	std::mutex usersMutex;
	std::map<std::shared_ptr<User>, std::string> tempSaltByUser;
	LogoutEvent::Observer logoutObserver;
	std::shared_ptr<User> users[MAX_PLAYERS_ONLINE];
	std::vector<int> availableIndices;
	std::vector<std::shared_ptr<User>> iteratableUsers;
	virtual void logout(const std::shared_ptr<User>& user);
	GameMessageEvent::Observer gameMessageObserver;
public:
	UserService(ServiceProvider* provider);
	virtual void init() override;
	virtual const std::shared_ptr<User>& getUserByIndex(int index) override;
	virtual const std::vector<std::shared_ptr<User>>& getAllUsers() override;
};
