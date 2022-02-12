#include "map.h"
#include "userService.h"
#include "socketServer.h"
#include "playerActionService.h"
#include "app.h"
#include "tickScheduler.h"
#include "taskScheduler.h"
#include "objectFactory.h"
#include "serverServiceProvider.h"
#include "objectService.h"
#include "onExit.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define IN_MEMORY_DATABASE


#ifdef IN_MEMORY_DATABASE
#include "inMemoryDB.h"
#else
#include "db.h"
#endif

int main() {
	srand(time(NULL));

	ServerServiceProvider provider;
#ifdef IN_MEMORY_DATABASE
	InMemoryDB db(&provider);
#else
	DB db(&provider, 5);
#endif
	ObjectFactory objf;
	Map map(&provider, objf);
	UserService userService(&provider);
	PlayerActionService playerActionService(&provider);
	SocketServerService server(&provider, 38838);
	ClockTickScheduler tickScheduler;
	TaskScheduler scheduler(&provider);
	ObjectService os(&provider);
	App app(&provider, &tickScheduler);
	app.init();
	app.start();
}
