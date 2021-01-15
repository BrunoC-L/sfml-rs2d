#include "pch.h"
#include "main.h"
#include "socketMock1.h"

sf::RenderWindow sfWindow(
	sf::VideoMode(
		AbstractMeasures::startingScreenSize().x,
		AbstractMeasures::startingScreenSize().y
	),
	"RS2D"
);

TEST(app_builds, TestName) {
	AbstractServiceProvider& provider = AbstractServiceProvider();
	AbstractSocket& socket = Socket(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = Map(&provider);
	AbstractInventory& inventory = Inventory(&provider);
	AbstractGameDataService& gameData = GameDataService(&provider);

	TickScheduler& scheduler = ClockTickScheduler();
	AbstractRenderWindow& window = SFRenderWindow(&provider, &scheduler, sfWindow);
	App app(&provider, &window, &socket, &measures, &map, &player, &camera, &chat, &inventory, &gameData);

	sfWindow.setActive(false);
}

TEST(app_runs_with_socket_mock_1, TestName) {
	AbstractServiceProvider& provider = AbstractServiceProvider();
	AbstractSocket& socket = SocketMock1(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = Map(&provider);
	AbstractInventory& inventory = Inventory(&provider);
	AbstractGameDataService& gameData = GameDataService(&provider);

	TickScheduler& scheduler = ClockTickScheduler();
	AbstractRenderWindow& window = SFRenderWindow(&provider, &scheduler, sfWindow);


	App app(&provider, &window, &socket, &measures, &map, &player, &camera, &chat, &inventory, &gameData);

	EXPECT_EQ(app.player->position, VTile());

	bool hasStarted = false;

	sfWindow.setActive(false);
	std::thread t(
		[&]() {
			sfWindow.setActive(true);
			app.init();
			hasStarted = true;
			app.start();
			sfWindow.setActive(false);
		}
	);

	while (!hasStarted);

	std::this_thread::sleep_for(20ms);

	app.stop();
	t.join();
}

TEST(player_position_updates_when_server_emits, TestName) {
	AbstractServiceProvider& provider = AbstractServiceProvider();
	SocketMock1& socket = SocketMock1(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = Map(&provider);
	AbstractInventory& inventory = Inventory(&provider);
	AbstractGameDataService& gameData = GameDataService(&provider);

	TickScheduler& scheduler = ClockTickScheduler();
	AbstractRenderWindow& window = SFRenderWindow(&provider, &scheduler, sfWindow);

	App app(&provider, &window, &socket, &measures, &map, &player, &camera, &chat, &inventory, &gameData);

	EXPECT_EQ(app.player->position, VTile());

	bool hasStarted = false;

	std::thread t(
		[&]() {
			app.init();
			hasStarted = true;
			app.start();
		}
	);

	while (!hasStarted);

	JSON hello;
	hello["type"] = "'hello'";
	hello["data"] = "7";

	socket.mockReceiveFromServer(hello);

	VTile lumbridge(18 * AbstractMeasures::TilesPerChunk + 20, 13 * AbstractMeasures::TilesPerChunk + 37, 0);
	EXPECT_EQ(app.player->position, lumbridge);
	
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

	EXPECT_EQ(app.player->position, lumbridge);

	app.stop();
	t.join();
}
