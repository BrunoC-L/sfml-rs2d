#include "gameDataService.h"
#include "constants.h"
#include "chunk.h"
#include "session.h"

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
		int* objects = new int[TILES_PER_CHUNK * TILES_PER_CHUNK](); // mem leak :) ?
		std::vector<std::pair<VTile, ObjectInteractions>> v;
		//v.reserve(data.get("size").asInt());
		for (const auto& object : data.get("objects").getChildren())
			for (const auto& e : parseObject(object, chunk)) {
				objects[int(TILES_PER_CHUNK * e.first.x + e.first.y)] = e.second.first;
				v.push_back({ e.first, e.second.second });
			}
		objectsReceived.push_back({ chunk, {objects, v} });
	});

	socket->on("objectUpdates", [&](std::shared_ptr<const JSON> json) {
		const JSON& data = *json;
		VChunk chunk(
			data.get("cx").asInt(),
			data.get("cy").asInt(),
			data.get("cz").asInt()
		);
		for (const auto& object : data.get("objects").getChildren())
			for (auto& e : parseObject(object, chunk)) {
				map->getChunk(chunk).objectMap.update(e.first, e.second.first);
				map->updateInteractions(chunk, e.first, &e.second.second);
			}
	});

	loginObserver.set([&](const LoginEvent::Data& ev) {
		loggedIn = true;
		// Temporal link with the player subscription...
		// maybe add an event called when the player receives login
		// the other solution is for the GameDataService to own the Player
		playerPositions = std::make_unique<PlayerPositions>(player);
	});

	logoutObserver.set([&](const LogoutEvent::Data& ev) {
		loggedIn = false;
		playerPositions.reset();
	});
}

const bool& GameDataService::userIsLoggedIn() {
    return loggedIn;
}

std::vector<playerIdAndPosition> GameDataService::getPlayerPositions() {
	if (playerPositions == nullptr)
		throw std::runtime_error("Asking for positions before login");
    return playerPositions->getPlayerPositions(tracker->getTickFraction());
}

std::pair<int*, std::vector<std::pair<VTile, ObjectInteractions>>> GameDataService::requestObjectsForChunk(VChunk chunk) {
	while (true)
		for (int i = 0; i < objectsReceived.size(); ++i) {
			const auto& pair = objectsReceived[i];
			if (pair.first == chunk) {
				auto temp = pair.second;
				objectsReceived.erase(objectsReceived.begin() + i);
				return temp;
			}
		}
}

void GameDataService::clearObjectsCache() {
	for (auto& objectReceived : objectsReceived)
		delete[] objectReceived.second.first;
	objectsReceived = {};
}

void GameDataService::storePositions(const JSON& json) {
	if (playerPositions == nullptr)
		throw std::runtime_error("Giving positions before login");
    playerPositions->update(json);
}

std::vector<std::pair<VTile, std::pair<int, ObjectInteractions>>> GameDataService::parseObject(const JSON& object, VChunk chunk) {
	std::vector<std::pair<VTile, std::pair<int, ObjectInteractions>>> res;
	int x = object.get("x").asInt() % int(TILES_PER_CHUNK);
	int y = object.get("y").asInt() % int(TILES_PER_CHUNK);
	std::vector<std::string> interactions;
	for (const auto& i : object.get("interactions").getChildren())
		interactions.push_back(i.asString());
	interactions.push_back("Examine");
	ObjectInteractions oi(VTile(TILES_PER_CHUNK * chunk.x + x, TILES_PER_CHUNK * chunk.y + y), object.get("name").asString(), interactions, object.get("state").asInt());

	const auto& size = object.get("size").getChildren();
	int sizeX = size[0].asInt();
	int sizeY = size[1].asInt();
	for (int dx = 0; dx < sizeX; ++dx)
		for (int dy = 0; dy < sizeY; ++dy)
			res.push_back({ VTile(x + dx, y + dy), { object.get("textures").get(dx * sizeY + dy).asInt(), oi} });
	return res;
}