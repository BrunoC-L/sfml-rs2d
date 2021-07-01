#include "pch.h"
#include "main.h"
#include "socketMock1.h"
#include "frameEvent.h"
#include "onExit.h"
#include "mapMock1.h"
#include "windowMock1.h"
#include "constants.h"

TEST(player_position_updates_on_socket_receive_positions, TestName) {
	ServiceProvider& provider = ClientServiceProvider();
	SocketMock1& socket = SocketMock1(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = MapMock(&provider);
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

	VTile lumbridge(1172, 869, 0);

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

TEST(PlayerPositions_updates_as_expected, TestName) {
	ServiceProvider& provider = ClientServiceProvider();
	SocketMock1& socket = SocketMock1(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = MapMock(&provider);
	GameTickProgress& tracker = ClockGameTickProgress();
	AbstractGameDataService& gameData = GameDataService(&provider, &tracker);

	measures.init();
	player.init();
	camera.init();
	map.init();
	chat.init();
	gameData.init();
	socket.init();

	socket.mockReceiveFromServer(JSON("{'type':'login', 'data':{'id':3, 'position':{'x':123,'y':456}}}"));

	PlayerPositions p(&player);
	p.update(JSON("[]"));
	EXPECT_EQ(p.getPlayerPositions(0).size(), 0);
	p.update(JSON("[\
		{\
			'x': 1,\
			'y': 1,\
			'id': 1\
		}\
	]"));
	EXPECT_EQ(p.getPlayerPositions(0).size(), 1);
	EXPECT_EQ(player.getIntPosition(), VTile(123, 456));
	p.update(JSON("[\
		{\
			'x': 123,\
			'y': 456,\
			'id': 3\
		}\
	]"));
	p.update(JSON("[\
		{\
			'x': 321,\
			'y': 654,\
			'id': 3\
		}\
	]"));
	p.getPlayerPositions(0);
	EXPECT_EQ(player.getIntPosition(), VTile(321, 654));
	EXPECT_EQ(player.getPosition(), VTile(123, 456));
	p.getPlayerPositions(0.5);
	EXPECT_EQ(player.getIntPosition(), VTile(321, 654));
	EXPECT_EQ(player.getPosition(), VTile(222, 555));
	p.getPlayerPositions(1);
	EXPECT_EQ(player.getIntPosition(), VTile(321, 654));
	EXPECT_EQ(player.getPosition(), VTile(321, 654));
}
