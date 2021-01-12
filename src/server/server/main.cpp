#include "map.h"
#include "db.h"
#include "userService.h"
#include "socketServer.h"
#include "playerActionService.h"
#include "tickScheduler.h"
#include "app.h"

int main() {
	App<
		Map,
		DB,
		UserService,
		PlayerActionService,
		SocketServerService,
		ClockTickScheduler
	> app(
		38838,
		(wchar_t*)L"DRIVER=SQL Server Native Client 11.0;SERVER=DESKTOP-FJJ4HB5\\SQLEXPRESS;DATABASE=rs2d;Trusted_Connection=Yes;"
	);
	app.init();
	app.start();
	app.stop();
}
