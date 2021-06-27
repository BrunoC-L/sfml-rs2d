#include "pch.h"
#include "main.h"
#include "globalWindow.h"
#include "socketMock1.h"
#include "frameEvent.h"
#include "onExit.h"
#include "mapMock1.h"

TEST(app_builds, TestName) {
	globalWindow->setActive(false);
	ServiceProvider& provider = ClientServiceProvider();
	AbstractSocket& socket = Socket(&provider, "1", 1);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = Map(&provider);
	AbstractInventory& inventory = Inventory(&provider);
	GameTickProgress& tracker = ClockGameTickProgress();
	AbstractGameDataService& gameData = GameDataService(&provider, &tracker);

	AbstractRenderWindow& window = SFRenderWindow(&provider, *globalWindow);

	App app(&provider, &window);
}

TEST(app_runs_with_socket_mock_1, TestName) {
	globalWindow->setActive(false);
	ServiceProvider& provider = ClientServiceProvider();
	AbstractSocket& socket = SocketMock1(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = Map(&provider);
	AbstractInventory& inventory = Inventory(&provider);
	GameTickProgress& tracker = ClockGameTickProgress();
	AbstractGameDataService& gameData = GameDataService(&provider, &tracker);

	AbstractRenderWindow& window = SFRenderWindow(&provider, *globalWindow);

	App app(&provider, &window);

	bool hasStarted = false;
	bool done = false;

	std::thread t(
		[&]() {
			OnExit _([&]() { done = true; });
			globalWindow->setActive(true);
			app.init();
			hasStarted = true;
			app.start();
			globalWindow->setActive(false);
		}
	);

	while (!hasStarted && !done); // if something goes wrong, done will go through
	std::this_thread::sleep_for(std::chrono::seconds(1));
	app.stop();
	t.join();
}

TEST(player_position_updates_when_server_emits, TestName) {
	globalWindow->setActive(false);
	ServiceProvider& provider = ClientServiceProvider();
	SocketMock1& socket = SocketMock1(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = MapMock(&provider);
	AbstractInventory& inventory = Inventory(&provider);
	GameTickProgress& tracker = ClockGameTickProgress();
	AbstractGameDataService& gameData = GameDataService(&provider, &tracker);

	AbstractRenderWindow& window = SFRenderWindow(&provider, *globalWindow);

	App app(&provider, &window);

	bool hasStarted = false;
	bool done = false;

	std::thread t(
		[&]() {
			OnExit _([&]() { done = true; });
			globalWindow->setActive(true);
			app.init();
			hasStarted = true;
			app.start();
			globalWindow->setActive(false);
		}
	);

	while (!hasStarted && !done); // if something goes wrong, done will go through

	auto _1172 = 18 * TILES_PER_CHUNK + 20;
	auto _869 = 13 * TILES_PER_CHUNK + 37;
	VTile lumbridge(_1172, _869, 0);

	auto close = lumbridge + VTile(3, -3);

	JSON hello;
	int id = 7;
	hello["type"] = "login";
	hello["data"] = JSON();
	hello["data"]["position"] = JSON();
	hello["data"]["position"]["x"] = std::to_string(close.x);
	hello["data"]["position"]["y"] = std::to_string(close.y);
	hello["data"]["id"] = std::to_string(id);
	socket.mockReceiveFromServer(hello);
	EXPECT_EQ(player.getID(), id);
	EXPECT_EQ(player.getPosition(), close);
	EXPECT_EQ(player.getIntPosition(), close);
	 
	close = lumbridge + VTile(1, -1);

	JSON data("[]");
	JSON playerPos;
	playerPos["id"] = std::to_string(id);
	playerPos["x"] = std::to_string(close.x);
	playerPos["y"] = std::to_string(close.y);
	data.push(playerPos);
	JSON json;
	json["data"] = data;
	json["type"] = "positions";
	socket.mockReceiveFromServer(json);

	int frames = 0;

	EventObserver<FrameEvent> frameObserver;
	frameObserver.set([&](FrameEvent& ev) {
		++frames;
	});

	while (!frames);
	VTile playerIntPos = player.getIntPosition();
	bool eq = playerIntPos == close;
	EXPECT_EQ(playerIntPos, close);

	try {
		app.stop();
	}
	catch (std::exception e) {
		std::cout << e.what();
		EXPECT_EQ(1, 0);
	}
	t.join();
}
