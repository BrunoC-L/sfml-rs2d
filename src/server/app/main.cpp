#include "main.h"

int main() {
	App<
		Map,
		DB,
		UserService,
		PlayerActionService,
		SocketServerService,
		ClockTickScheduler
	> app(
		38838
	);
	app.init();
	app.start();
	app.stop();
}