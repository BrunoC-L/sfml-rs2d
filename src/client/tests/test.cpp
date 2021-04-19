#include "pch.h"
#include "main.h"
#include "globalWindow.h"
#include "socketMock1.h"
#include "frameEvent.h"

TEST(app_builds, TestName) {
	globalWindow->setActive(false);
	AbstractServiceProvider& provider = AbstractServiceProvider();
	AbstractSocket& socket = Socket(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = Map(&provider);
	AbstractInventory& inventory = Inventory(&provider);
	GameTickProgress& tracker = ClockGameTickProgress();
	AbstractGameDataService& gameData = GameDataService(&provider, &tracker);

	AbstractRenderWindow& window = SFRenderWindow(&provider, *globalWindow);
	App app(&provider, &window, &socket, &measures, &map, &player, &camera, &chat, &inventory, &gameData);
}

TEST(app_runs_with_socket_mock_1, TestName) {
	globalWindow->setActive(false);
	AbstractServiceProvider& provider = AbstractServiceProvider();
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


	App app(&provider, &window, &socket, &measures, &map, &player, &camera, &chat, &inventory, &gameData);

	EXPECT_EQ(app.player->getPosition(), VTile());

	bool hasStarted = false;

	std::thread t(
		[&]() {
			globalWindow->setActive(true);
			app.init();
			hasStarted = true;
			app.start();
			globalWindow->setActive(false);
		}
	);

	while (!hasStarted);

	app.stop();
	t.join();
}

TEST(player_position_updates_when_server_emits, TestName) {
	globalWindow->setActive(false);
	AbstractServiceProvider& provider = AbstractServiceProvider();
	SocketMock1& socket = SocketMock1(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = Map(&provider);
	AbstractInventory& inventory = Inventory(&provider);
	GameTickProgress& tracker = ClockGameTickProgress();
	AbstractGameDataService& gameData = GameDataService(&provider, &tracker);

	AbstractRenderWindow& window = SFRenderWindow(&provider, *globalWindow);

	App app(&provider, &window, &socket, &measures, &map, &player, &camera, &chat, &inventory, &gameData);

	EXPECT_EQ(app.player->getPosition(), VTile());

	bool hasStarted = false;

	std::thread t(
		[&]() {
			globalWindow->setActive(true);
			app.init();
			hasStarted = true;
			app.start();
			globalWindow->setActive(false);
		}
	);

	while (!hasStarted);

	JSON hello;
	int id = 7;
	hello["type"] = "'login'";
	hello["data"] = std::to_string(id);
	socket.mockReceiveFromServer(hello);
	EXPECT_EQ(player.getID(), id);

	auto _1172 = 18 * AbstractMeasures::TilesPerChunk + 20;
	auto _869 = 13 * AbstractMeasures::TilesPerChunk + 37;
	VTile lumbridge(_1172, _869, 0);
	EXPECT_EQ(player.getPosition(), lumbridge);

	auto close = lumbridge + VTile(1, -1);

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
	FrameEvent::subscribe(
		new EventObserver<FrameEvent>(
			[&](FrameEvent* ev) {
				++frames;
			}
		)
	);

	while (!frames);

	EXPECT_EQ(player.getPosition(), close);

	app.stop();
	t.join();
}
