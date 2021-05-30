#include "gameDataService.h"
#include "constants.h"
#include "chunk.h"

GameDataService::GameDataService(ServiceProvider* provider, GameTickProgress* tracker) : Service(provider), tracker(tracker) {
    provider->set(GAMEDATA, this);
	std::ifstream name2textureIndex("../../../resource/objects/objectName2texture.txt");
	if (!name2textureIndex.is_open())
		throw std::exception("Failed to open ../../../resource/objects/objectName2texture.txt");
	std::string nameOfTexture;
	while (std::getline(name2textureIndex, nameOfTexture)) {
		auto content = split(nameOfTexture, ":");
		name2texture[content[0] + content[1]] = stoi(content[2]);
	}
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
		for (const auto& object : data.get("objects").getChildren())
			for (const auto& e : parseObject(object, chunk)) {
				objects[int(AbstractMeasures::TilesPerChunk * e.first.x + e.first.y)] = e.second.first;
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
		std::vector<std::pair<VTile, ObjectInteractions>> v;
		for (const auto& object : data.get("objects").getChildren())
			for (auto& e : parseObject(object, chunk)) {
				map->getChunk(chunk).objectMap.update(e.first, e.second.first);
				map->updateInteractions(chunk, e.first, &e.second.second);
			}
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

std::vector<std::pair<VTile, std::pair<int, ObjectInteractions>>> GameDataService::parseObject(const JSON& object, VChunk chunk) {
	std::vector<std::pair<VTile, std::pair<int, ObjectInteractions>>> res;
	int x = object.get("x").asInt() % int(AbstractMeasures::TilesPerChunk);
	int y = object.get("y").asInt() % int(AbstractMeasures::TilesPerChunk);
	std::vector<std::string> interactions;
	for (const auto& i : object.get("interactions").getChildren())
		interactions.push_back(i.asString());
	interactions.push_back("Examine");
	ObjectInteractions oi(VTile(AbstractMeasures::TilesPerChunk * chunk.x + x, AbstractMeasures::TilesPerChunk * chunk.y + y), object.get("name").asString(), interactions, object.get("state").asInt());
	int sizeX = object.get("size").getChildren()[0].asInt();
	int sizeY = object.get("size").getChildren()[1].asInt();
	std::string fileName = object.get("fileName").asString();
	std::string nameInFile = fileName + "-" + object.get("state").asString() + " 0-0";
	int c = name2texture[nameInFile];
	for (int dx = 0; dx < sizeX; ++dx)
		for (int dy = 0; dy < sizeY; ++dy)
			res.push_back({ VTile(x + dx, y + dy), { c + sizeY * dx + dy, oi} });
	return res;
}
