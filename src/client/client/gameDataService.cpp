#include "gameDataService.h"
#include "constants.h"

GameDataService::GameDataService(ServiceProvider* provider, GameTickProgress* tracker) : Service(provider), tracker(tracker) {
    provider->set(GAMEDATA, this);
}

void GameDataService::init() {
    acquire();
    socket->on("positions", [&](std::shared_ptr<const JSON> json) {
        storePositions(*json);
        tracker->onGameTick();
    });
	socket->on("objectStates", [&](std::shared_ptr<const JSON> json) {
		const JSON& data = *json;
		VChunk chunk(
			data.get("cx").asInt(),
			data.get("cy").asInt(),
			data.get("cz").asInt()
		);
		int* objects = new int[AbstractMeasures::TilesPerChunk * AbstractMeasures::TilesPerChunk]();
		std::vector<std::pair<VTile, ObjectInteractions>> v;
		for (const auto& object : data.get("objects").getChildren()) {
			int x = object.get("x").asInt() % int(AbstractMeasures::TilesPerChunk);
			int y = object.get("y").asInt() % int(AbstractMeasures::TilesPerChunk);
			std::vector<std::string> interactions;
			for (const auto& i : object.get("interactions").getChildren())
				interactions.push_back(i.asString());
			ObjectInteractions oi(VTile(AbstractMeasures::TilesPerChunk * chunk.x + x, AbstractMeasures::TilesPerChunk * chunk.y + y), object.get("name").asString(), interactions, object.get("state").asInt());
			if (object.get("name").asString() == "Tree") {
				int c = 3;
				for (int dx = 0; dx < 2; ++dx) for (int dy = 0; dy < 2; ++dy) {
					VTile tile(x + dx, y + dy);
					objects[int(AbstractMeasures::TilesPerChunk) * int(tile.x) + int(tile.y)] = c + 2 * dx + dy;
					v.push_back({ tile, oi });
				}
			}
		}
		objectsReceived.push_back({ chunk, {objects, v} });
	});

	loginObserver.set([&](LoginEvent& ev) {
		loggedIn = true;
		// Temporal link with the player subscription...
		// maybe add an event called when the player receives login
		// the other solution is for the GameDataService to own the Player
		playerPositions = std::make_unique<PlayerPositions>(player);
	});

	logoutObserver.set([&](LogoutEvent& ev) {
		loggedIn = false;
		playerPositions.reset();
	});
}

const bool& GameDataService::userIsLoggedIn() {
    return loggedIn;
}

std::vector<playerIdAndPosition> GameDataService::getPlayerPositions() {
	if (playerPositions == nullptr)
		throw std::exception("Asking for positions before login");
    return playerPositions->getPlayerPositions(tracker->getTickFraction());
}

std::pair<int*, std::vector<std::pair<VTile, ObjectInteractions>>> GameDataService::requestObjectsForChunk(VChunk chunk) {
	for (int i = 0; i < objectsReceived.size(); ++i) {
		const auto& pair = objectsReceived[i];
		if (pair.first == chunk) {
			auto temp = pair.second;
			objectsReceived.erase(objectsReceived.begin() + i);
			return temp;
		}
	}
	throw std::exception();
}

void GameDataService::clearObjectsCache() {
	std::cout << "Clear\n";
	for (int i = objectsReceived.size() - 1; i >= 0; --i) {
		delete[] objectsReceived[i].second.first;
		objectsReceived.erase(objectsReceived.begin() + i);
	}
}

void GameDataService::storePositions(const JSON& json) {
	if (playerPositions == nullptr)
		throw std::exception("Giving positions before login");
    playerPositions->update(json);
}
