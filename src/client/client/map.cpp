#include "map.h"
#include <iostream>
#include "print.h"

Map::Map(ServiceProvider* provider, int chunkRadius) : Service(provider), chunkRadius(chunkRadius) {
	provider->set("Map", this);
};

void Map::init() {
	acquire();
	const VTile& pos = camera->getPosition();
	centerChunk = VChunk(int(pos.x / AbstractMeasures::TilesPerChunk), int(pos.y / AbstractMeasures::TilesPerChunk), int(pos.z));
	doUpdates();
}

void Map::load() {
	auto diameter = 2 * chunkRadius + 1;
	loaded = std::vector<std::vector<std::shared_ptr<Chunk>>>(diameter, std::vector<std::shared_ptr<Chunk>>(diameter, nullptr));
	std::lock_guard<std::mutex> lock(mutex);
	for (int i = 0; i < diameter; ++i)
		for (int j = 0; j < diameter; ++j)
			loaded[i][j] = std::make_shared<Chunk>(centerChunk + VChunk(i, j) - VChunk(chunkRadius, chunkRadius));
}

void Map::update() {
	const VTile pos = camera->getPosition() + VTile(0.5, 0.5); // to account for player centered on tile
	const VChunk newChunk(int(pos.x / AbstractMeasures::TilesPerChunk), int(pos.y / AbstractMeasures::TilesPerChunk), int(pos.z));
	const VChunk difference = newChunk - centerChunk;
	if (!difference.x && !difference.y && !difference.z)
		return;
	updateChunks(difference, newChunk);
	centerChunk = newChunk;
}

void Map::updateChunks(const VChunk& difference, const VChunk& tempCenter) {
	auto diameter = 2 * chunkRadius + 1;
	std::vector<std::vector<std::shared_ptr<Chunk>>> newChunks(diameter, std::vector<std::shared_ptr<Chunk>>(diameter, nullptr));
	for (int i = 0; i < diameter; ++i)
		for (int j = 0; j < diameter; ++j)
			if (i + difference.x >= 0 && i + difference.x < diameter && j + difference.y >= 0 && j + difference.y < diameter)
				newChunks[i][j] = loaded[i + difference.x][j + difference.y];
			else
				newChunks[i][j] = std::make_shared<Chunk>(tempCenter + VChunk(i, j) - VChunk(chunkRadius, chunkRadius));
	std::lock_guard<std::mutex> lock(mutex);
	std::swap(loaded, newChunks);
}

void Map::doUpdates() {
	updateThread = std::thread(
		[&]() {
			{
				std::ostringstream ss;
				ss << "Main Thread: " << std::this_thread::get_id() << std::endl;
				print(ss);
			}
			load();
			while (!shouldStop) {
				update();
			}
			{
				std::ostringstream ss;
				ss << "Main Thread: " << std::this_thread::get_id() << " Exiting" << std::endl;
				print(ss);
			}
		}
	);
}

std::shared_ptr<Tile> Map::getTileFromVTile(VTile tilePosition) {
	VChunk chunkOfTileClicked = VChunk(int(tilePosition.x / AbstractMeasures::TilesPerChunk), int(tilePosition.y / AbstractMeasures::TilesPerChunk));
	VChunk deltaChunkOffsetWithMiddleChunk = chunkOfTileClicked - centerChunk + VChunk(loaded.size() / 2, loaded.size() / 2);
	std::shared_ptr<Tile> t = nullptr;
	if (deltaChunkOffsetWithMiddleChunk.x >= 0 && deltaChunkOffsetWithMiddleChunk.x < loaded.size() &&
		deltaChunkOffsetWithMiddleChunk.y >= 0 && deltaChunkOffsetWithMiddleChunk.y < loaded.size()) {
		std::shared_ptr<Chunk> chunk = loaded[deltaChunkOffsetWithMiddleChunk.x][deltaChunkOffsetWithMiddleChunk.y];
		t = chunk->tiles[int(tilePosition.x - chunkOfTileClicked.x * AbstractMeasures::TilesPerChunk)][int(tilePosition.y - chunkOfTileClicked.y * AbstractMeasures::TilesPerChunk)];
	}
	return t;
}

void Map::stopUpdates() {
	print("Stopping map\n");
	this->shouldStop = true;
	updateThread.join();
}

unsigned Map::getRadius() {
	return chunkRadius;
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
