#include "pch.h"
#include "main.h"
#include "OnExit.h"

TEST(App_builds, TestName) {
	ServerServiceProvider provider;
	DB db(&provider, 5);
	ObjectFactory objf;
	Map map(&provider, objf);
	UserService userService(&provider);
	PlayerActionService playerActionService(&provider);
	SocketServerService server(&provider, 38838);
	ClockTickScheduler tickScheduler;
	TaskScheduler scheduler(&provider);
	App app(&provider, &server, &db, &map, &playerActionService, &userService, &tickScheduler, &scheduler);
}

TEST(SQLDB_App_inits_starts_stops, TestName) {
	ServerServiceProvider provider;
	DB db(&provider, 5);
	ObjectFactory objf;
	Map map(&provider, objf);
	UserService userService(&provider);
	PlayerActionService playerActionService(&provider);
	SocketServerService server(&provider, 38838);
	ClockTickScheduler tickScheduler;
	TaskScheduler scheduler(&provider);
	App app(&provider, &server, &db, &map, &playerActionService, &userService, &tickScheduler, &scheduler);

	bool hasStarted = false;
	bool done = false;

	std::thread t(
		[&]() {
			OnExit _([&]() { done = true; });
			app.init();
			hasStarted = true;
			app.start();
		}
	);

	while (!hasStarted && !done); // if something goes wrong, done will go through
	std::this_thread::sleep_for(std::chrono::seconds(1));
	app.stop();
	t.join();
	EXPECT_TRUE(true);
}
