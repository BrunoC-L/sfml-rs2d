#include "pch.h"
#include "main.h"
#include "socketMock1.h"
#include "frameEvent.h"
#include "onExit.h"
#include "mapMock1.h"
#include "windowMock1.h"

TEST(player_position_updates_when_server_emits, TestName) {
	ServiceProvider& provider = ClientServiceProvider();
	SocketMock1& socket = SocketMock1(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = MapMock(&provider, 1);
	GameTickProgress& tracker = ClockGameTickProgress();
	AbstractGameDataService& gameData = GameDataService(&provider, &tracker);

	AbstractRenderWindow& window = MockWindow();

	App app(&provider, &window);

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

	auto _1172 = 18 * AbstractMeasures::TilesPerChunk + 20;
	auto _869 = 13 * AbstractMeasures::TilesPerChunk + 37;
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
