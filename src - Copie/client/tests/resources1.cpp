#include "pch.h"
#include "main.h"
#include "socketMock2.h"
#include "frameEvent.h"
#include "onExit.h"
#include "windowMock1.h"
#include "constants.h"
#include "random.h"
#include "globalWindow.h"
#include "mapUpdatedChunksEvent.h"

TEST(clicking_on_resources_received_from_server_sends_correct_interaction_current, TestName) {
	globalWindow->setActive(false);
	ServiceProvider& provider = ClientServiceProvider();
	SocketMock2& socket = SocketMock2(&provider);
	AbstractMeasures& measures = Measures(&provider);
	AbstractChat& chat = Chat(&provider);
	AbstractCamera& camera = Camera(&provider);
	AbstractPlayer& player = Player(&provider);
	AbstractMap& map = Map(&provider);
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
			std::cout << "APP thread stopped\n";
		}
	);

	while (!hasStarted && !done); // if something goes wrong, done will go through

	int messagesReceived = 0;
	bool testWorked = false;
	bool testFailed = false;
	bool receivedMapUpdateEvent = false;
	int cx = randint(CHUNK_RADIUS, 29 - CHUNK_RADIUS), cy = randint(CHUNK_RADIUS, 25 - CHUNK_RADIUS);
	int x = randint(TILES_PER_CHUNK), y = randint(TILES_PER_CHUNK);

	JSON object;
	object["x"] = std::to_string(x);
	object["y"] = std::to_string(y);
	object["size"] = "[2, 2]";
	object["state"] = "0";
	std::string bonk = "bonk";
	object["interactions"] = "[" + bonk + "]";
	object["fileName"] = "Tree";
	std::string resourceName = "Tree";
	object["name"] = resourceName;
	MapUpdatedChunksEvent::Observer obv;

	socket._send = [&](const std::string& type, const JSON& json) {};

	socket._send = [&](const std::string& type, const JSON& json) {
		switch (messagesReceived++) {
			case 0: // when client sends salts request
				socket.mockReceiveFromServer("salts", JSON("{'tempsalt':'123','permsalt':'321'}"));
				break;
			case 1: // when client sends login
				socket.mockReceiveFromServer("login",
					JSON("{'id': 123,'position': {'x': " +
						std::to_string(TILES_PER_CHUNK * cx + x) +
						", 'y':" +
						std::to_string(TILES_PER_CHUNK * cy + y) +
						"}}"
					)
				);
				for (int dx = -CHUNK_RADIUS; dx <= CHUNK_RADIUS; ++dx)
					for (int dy = -CHUNK_RADIUS; dy <= CHUNK_RADIUS; ++dy)
						socket.mockReceiveFromServer(
							"objectStates",
							JSON(
								"{'cx':" +
								std::to_string(cx + dx) +
								", 'cy':" +
								std::to_string(cy + dy) +
								", 'cz':" +
								std::to_string(0) +
								", 'objects':[" +
								object.asString() +
								"]}"
							)
						);

				obv.set([&](const MapUpdatedChunksEvent::Data& ev) {
					receivedMapUpdateEvent = true;
				});
				for (int i = 0; i < 2000 && !receivedMapUpdateEvent; ++i) {
					using namespace std::chrono_literals;
					std::this_thread::sleep_for(1ms);
				}
				if (!receivedMapUpdateEvent)
					testFailed = true;
				else {
					EVENT(MouseRightClickEvent, AbstractMeasures::startingInnerScreenSize() / 2).emit();
					EVENT(MouseLeftClickEvent, AbstractMeasures::startingInnerScreenSize() / 2 + VPixel(25, 25)).emit();
				}
				break;
			case 2: // when client sends interact
				if (json.get("object").get("interaction").asString() == bonk && json.get("object").get("objectName").asString() == resourceName)
					testWorked = true;
				break;
			default:
				testFailed = true;
				throw std::runtime_error("Test Failed");
		}
	};

	for (const auto key : "username")
		EVENT(LetterKeyPressedEvent, key, false).emit();
	EVENT(EnterKeyPressedEvent).emit();
	for (const auto key : "password")
		EVENT(LetterKeyPressedEvent, key, false).emit();
	EVENT(EnterKeyPressedEvent).emit();
	for (int i = 0; i < 5000 && !testWorked && !testFailed; ++i) {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1ms);
	}

	try {
		EXPECT_TRUE(testWorked);
		EXPECT_FALSE(testFailed);
		app.stop();
		t.join();
	}
	catch (std::exception& e) {
		std::cout << e.what();
		EXPECT_TRUE(false);
	}
	EXPECT_TRUE(testWorked);
	EXPECT_FALSE(testFailed);
}
