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
	GameTickProgress& tracker = GameTickProgress();
	GameDataStorage& storage = GameDataStorage();
	AbstractGameDataService& gameData = GameDataService(&provider, &tracker, &storage);

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
	GameTickProgress& tracker = GameTickProgress();
	GameDataStorage& storage = GameDataStorage();
	AbstractGameDataService& gameData = GameDataService(&provider, &tracker, &storage);

	AbstractRenderWindow& window = SFRenderWindow(&provider, *globalWindow);


	App app(&provider, &window, &socket, &measures, &map, &player, &camera, &chat, &inventory, &gameData);

	EXPECT_EQ(app.player->position, VTile());

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

	std::this_thread::sleep_for(20ms);

	app.stop();
	t.join();
}

TEST(player_position_updates_when_server_emits_current, TestName) {
	globalWindow->setActive(false);
	AbstractServiceProvider& provider = AbstractServiceProvider();
	SocketMock1& socket = SocketMock1(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = Map(&provider);
	AbstractInventory& inventory = Inventory(&provider);
	GameTickProgress& tracker = GameTickProgress();
	GameDataStorage& storage = GameDataStorage();
	AbstractGameDataService& gameData = GameDataService(&provider, &tracker, &storage);

	AbstractRenderWindow& window = SFRenderWindow(&provider, *globalWindow);

	App app(&provider, &window, &socket, &measures, &map, &player, &camera, &chat, &inventory, &gameData);

	EXPECT_EQ(app.player->position, VTile());

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
	hello["type"] = "'hello'";
	hello["data"] = "7";

	socket.mockReceiveFromServer(hello);

	VTile lumbridge(18 * AbstractMeasures::TilesPerChunk + 20, 13 * AbstractMeasures::TilesPerChunk + 37, 0);
	EXPECT_EQ(player.position, lumbridge);
	
	lumbridge += VTile(1, -1);

	JSON json;
	json["type"] = "'positions'";
	JSON data("[]");
	JSON playerPos;
	playerPos["id"] = "7";
	playerPos["x"] = std::to_string(lumbridge.x);
	playerPos["y"] = std::to_string(lumbridge.y);
	data.push(playerPos);
	json["data"] = data;

	socket.mockReceiveFromServer(json);

	int frames = 0;
	FrameEvent::subscribe(
		new EventObserver<FrameEvent>(
			[&](FrameEvent* ev) {
				++frames;
			}
		)
	);

	while (frames < 2);

	EXPECT_EQ(player.position, lumbridge);

	app.stop();
	t.join();
}
