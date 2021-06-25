#include "service.h"
#include "services.h"

void Service::acquire() {
	map = static_cast<Map*>(provider->get(MAP));
	_ASSERT(map);
	dbService = static_cast<DB*>(provider->get(DATABASE));
	_ASSERT(dbService);
	userService = static_cast<UserService*>(provider->get(USER));
	_ASSERT(userService);
	playerActionService = static_cast<PlayerActionService*>(provider->get(PLAYERACTION));
	_ASSERT(playerActionService);
	server = static_cast<SocketServerService*>(provider->get(SERVER));
	_ASSERT(server);
	scheduler = static_cast<TaskScheduler*>(provider->get(SCHEDULER));
	_ASSERT(scheduler);
	resourceService = static_cast<ObjectService*>(provider->get(OBJECTS));
	_ASSERT(resourceService);
}
