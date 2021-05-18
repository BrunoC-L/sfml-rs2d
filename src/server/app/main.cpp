#include "main.h"
#include "onExit.h"

int main() {
	ServerServiceProvider provider;
	DB db(&provider, 5);
	ObjectFactory objf;
	Map map(&provider, objf);
	UserService userService(&provider);
	PlayerActionService playerActionService(&provider);
	SocketServerService server(&provider, 38838);
	ClockTickScheduler tickScheduler;
	TaskScheduler scheduler(&provider);
	ResourceService rs(&provider);
	App app(&provider, &tickScheduler);
	app.init();
	app.start();
}
