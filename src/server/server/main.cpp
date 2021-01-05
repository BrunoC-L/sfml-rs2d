#include "json-socket-server.h"
#include <sstream>
#include <chrono>
#include "pathfinder.h"
#include "map.h"
#include "app.h"

int main() {
	App<Map> app;
	app.init();
	app.start();
	app.stop();
}
