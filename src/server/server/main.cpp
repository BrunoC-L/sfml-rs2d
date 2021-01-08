#include "map.h"
#include "db.h"
#include "userService.h"
#include "socketServer.h"
#include "playerActionService.h"
#include "app.h"

int main() {
	App<Map, DB, UserService, SocketServerService, PlayerActionService> app;
	app.init();
	app.start();
	app.stop();
}