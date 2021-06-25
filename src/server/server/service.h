#pragma once
#include "serviceProvider.h"
#include "servicesEnum.h"

class Map;
class DB;
class UserService;
class PlayerActionService;
class SocketServerService;
class TaskScheduler;
class ObjectService;

class Service {
protected:
	ServiceProvider* provider = nullptr;
	Map* map = nullptr;
	DB* dbService = nullptr;
	UserService* userService = nullptr;
	PlayerActionService* playerActionService = nullptr;
	SocketServerService* server = nullptr;
	TaskScheduler* scheduler = nullptr;
	ObjectService* resourceService = nullptr;
public:
	Service(ServiceProvider* provider) : provider(provider) { }

	void acquire();
};
