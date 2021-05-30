#include "main.h"
#include "onExit.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(NULL));

	ServerServiceProvider provider;
	DB db(&provider, 5);
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
