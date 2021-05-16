#pragma once
#include "abstractServices.h"
#include "ServiceProvider.h"
#include "services.h"

class Service {
protected:
	ServiceProvider* provider = nullptr;
	AbstractMap* map = nullptr;
	AbstractDB* dbService = nullptr;
	AbstractUserService* userService = nullptr;
	AbstractPlayerActionService* playerActionService = nullptr;
	AbstractSocketServer* server = nullptr;
	AbstractTaskScheduler* scheduler = nullptr;
	AbstractResourceService* resourceService = nullptr;
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
		resourceService = (AbstractResourceService*)provider->get(RESOURCE);
		_ASSERT(map);
	}
};
