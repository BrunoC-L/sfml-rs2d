#include "main.h"

int main() {
	ServiceProvider provider;
	DB db(&provider);
	Map map(&provider);
	UserService userService(&provider);
	PlayerActionService playerActionService(&provider);
	SocketServerService server(&provider, 38838);
	ClockTickScheduler tickScheduler;
	TaskScheduler scheduler(&provider);
	App app(&provider, &server, &db, &map, &playerActionService, &userService, &tickScheduler, &scheduler);
	app.init();
	app.start();
	app.stop();
}
