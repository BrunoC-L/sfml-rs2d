#include "map.h"
#include <iostream>
#include "print.h"
#include "mapUpdatedChunksEvent.h"
#include "logger.h"
#include "session.h"

Map::Map(ServiceProvider* provider) : Service(provider) {
	provider->set(MAP, this);
	auto fileName = getSession().get("RS2D_HOME").asString() + "/assets/" + TEXTURES_FOLDER + "/objects.png";
	objectTileset.loadFromFile(fileName);

	loginObserver.set([&](const LoginEvent::Data& ev) {
		doUpdates();
	});

	logoutObserver.set([&](const LogoutEvent::Data& ev) {
		stopUpdates();
	});
};

void Map::init() {
	acquire();
	centerChunk = VChunk();
}

void Map::load() {
	auto log = Logging::clientDefaultFolderLogger("map.txt", true);
	log("acquiring map mutex");
	std::lock_guard<std::mutex> lock(mutex);
	isLoaded = true;
	while (camera->getPosition() == VChunk());
	centerChunk = VChunk(
		int(camera->getPosition().x / TILES_PER_CHUNK),
		int(camera->getPosition().y / TILES_PER_CHUNK),
		int(camera->getPosition().z / TILES_PER_CHUNK)
	);
	auto diameter = 2 * CHUNK_RADIUS + 1;
	loaded = std::vector<std::vector<std::shared_ptr<Chunk>>>(diameter, std::vector<std::shared_ptr<Chunk>>(diameter, nullptr));
	log("creating chunks");
	for (int i = 0; i < diameter; ++i)
		for (int j = 0; j < diameter; ++j)
			loaded[i][j] = std::make_shared<Chunk>(centerChunk + VChunk(i, j) - VChunk(CHUNK_RADIUS, CHUNK_RADIUS), &objectTileset, gameData);
	EVENT(MapUpdatedChunksEvent).emit();
}

void Map::update() {
	const VTile pos = player->getIntPosition();
	const VChunk newChunk(int(pos.x / TILES_PER_CHUNK), int(pos.y / TILES_PER_CHUNK), int(pos.z));
	const VChunk difference = newChunk - centerChunk;
	if (!difference.x && !difference.y && !difference.z)
		return;
	updateChunks(difference, newChunk);
}

void Map::updateChunks(const VChunk& difference, const VChunk& tempCenter) {
	auto diameter = 2 * CHUNK_RADIUS + 1;
	std::vector<std::vector<std::shared_ptr<Chunk>>> newChunks(diameter, std::vector<std::shared_ptr<Chunk>>(diameter, nullptr));
	for (int i = 0; i < diameter; ++i)
		for (int j = 0; j < diameter; ++j)
			if (i + difference.x >= 0 && i + difference.x < diameter && j + difference.y >= 0 && j + difference.y < diameter)
				newChunks[i][j] = loaded[i + difference.x][j + difference.y];
			else
				newChunks[i][j] = std::make_shared<Chunk>(tempCenter + VChunk(i, j) - VChunk(CHUNK_RADIUS, CHUNK_RADIUS), &objectTileset, gameData);
	std::lock_guard<std::mutex> lock(mutex);
	std::swap(loaded, newChunks);
	gameData->clearObjectsCache();
	centerChunk = tempCenter;
	EVENT(MapUpdatedChunksEvent).emit();
}

void Map::doUpdates() {
	initializing = true;
	shouldStop = false;
	auto log = Logging::clientDefaultFolderLogger("map.txt", true);
	log("starting update thread");
	updateThread = std::thread(
		[&, log]() {
			log("started update thread");
			{
				std::ostringstream ss;
				ss << "Map update thread: " << std::this_thread::get_id() << std::endl;
				print(ss);
			}
			log("loading map");
			load();
			log("loaded map");
			initializing = false;
			while (!shouldStop)
				update();
			isLoaded = false;
			{
				std::ostringstream ss;
				ss << "Map update thread: " << std::this_thread::get_id() << " Exiting" << std::endl;
				print(ss);
			}
		}
	);
}

void Map::stopUpdates() {
	auto log = Logging::clientDefaultFolderLogger("map.txt", true);
	log("stopping map updates");
	this->shouldStop = true;
	while (initializing);
	log("acquiring map mutex");
	std::lock_guard<std::mutex> lock(mutex);
	if (updateThread.joinable()) {
		log("joining update thread");
		updateThread.join();
	}
	log("stopped map updates");
}

std::shared_ptr<Tile> Map::getTileFromVTile(VTile tilePosition) {
	VChunk chunkOfTileClicked = VChunk(int(tilePosition.x / TILES_PER_CHUNK), int(tilePosition.y / TILES_PER_CHUNK));
	VChunk deltaChunkOffsetWithMiddleChunk = chunkOfTileClicked - centerChunk + VChunk(loaded.size() / 2, loaded.size() / 2);
	std::shared_ptr<Tile> t = nullptr;
	if (deltaChunkOffsetWithMiddleChunk.x >= 0 && deltaChunkOffsetWithMiddleChunk.x < loaded.size() &&
		deltaChunkOffsetWithMiddleChunk.y >= 0 && deltaChunkOffsetWithMiddleChunk.y < loaded.size()) {
		std::shared_ptr<Chunk> chunk = loaded[deltaChunkOffsetWithMiddleChunk.x][deltaChunkOffsetWithMiddleChunk.y];
		t = chunk->tiles[int(tilePosition.x - chunkOfTileClicked.x * TILES_PER_CHUNK)][int(tilePosition.y - chunkOfTileClicked.y * TILES_PER_CHUNK)];
	}
	return t;
}

VChunk Map::getCenterChunk() {
	return centerChunk;
}

Chunk& Map::getLoaded(int i, int j) {
	return *loaded[i][j];
}

std::mutex& Map::getChunksMutex() {
	return mutex;
}

bool Map::ready() {
	return isLoaded;
}

int* Map::getObjectsPtrForChunk(VChunk chunk) {
	return getChunk(chunk).getObjectsPtr();
}

void Map::updateInteractions(VChunk vc, VTile tile, ObjectInteractions* interactions) {
	auto& chunk = getChunk(vc);
	chunk.updateInteractions(tile, interactions);
}

Chunk& Map::getChunk(VChunk chunk) {
	while (initializing);
	std::lock_guard<std::mutex> lock(mutex);
	VChunk delta = chunk - centerChunk;
	int dx = int(delta.x);
	int dy = int(delta.y);
	int x = CHUNK_RADIUS + dx;
	int y = CHUNK_RADIUS + dy;
	return *loaded[x][y];
}
