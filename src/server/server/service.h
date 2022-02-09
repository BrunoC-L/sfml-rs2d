#pragma once
#include "abstractServices.h"
#include "serviceProvider.h"
#include "services.h"

#ifdef __APPLE__
#define _ASSERT assert
#endif // __APPLE__

class Service {
protected:
	ServiceProvider* provider = nullptr;
	AbstractMap* map = nullptr;
	AbstractDB* dbService = nullptr;
	AbstractUserService* userService = nullptr;
	AbstractPlayerActionService* playerActionService = nullptr;
	AbstractSocketServer* server = nullptr;
	AbstractTaskScheduler* scheduler = nullptr;
	AbstractObjectService* resourceService = nullptr;
public:
	Service(ServiceProvider* provider) : provider(provider) { }

	void acquire() {
		map = (AbstractMap*)provider->get(MAP);
		_ASSERT(map);
		dbService = (AbstractDB*)provider->get(DATABASE);
		_ASSERT(map);
		userService = (AbstractUserService*)provider->get(USER);
		_ASSERT(map);
		playerActionService = (AbstractPlayerActionService*)provider->get(PLAYERACTION);
		_ASSERT(map);
		server = (AbstractSocketServer*)provider->get(SERVER);
		_ASSERT(map);
		scheduler = (AbstractTaskScheduler*)provider->get(SCHEDULER);
		_ASSERT(map);
		resourceService = (AbstractObjectService*)provider->get(OBJECTS);
		_ASSERT(map);
	}
};
