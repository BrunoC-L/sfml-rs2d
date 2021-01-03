#include "map.h"
#include <iostream>

Map::Map(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("Map", this);
};

void Map::init() {
	acquire();
	VTile& pos = camera->getPosition();
	centerChunk = VChunk(int(pos.x / AbstractMeasures::TilesPerChunk), int(pos.y / AbstractMeasures::TilesPerChunk), int(pos.z));
	chunkRadius = 1;
	load();
}

void Map::load() {
	loaded = vector<vector<Chunk*>>(2 * chunkRadius + 1, vector<Chunk*>(2 * chunkRadius + 1, nullptr));
	mutex.lock();
	for (int i = 0; i < 2 * chunkRadius + 1; ++i)
		for (int j = 0; j < 2 * chunkRadius + 1; ++j) {
			VChunk chunkPos = centerChunk + VChunk(i, j) - VChunk(chunkRadius, chunkRadius);
			delete loaded[i][j];
			loaded[i][j] = new Chunk(chunkPos, provider);
		}
	mutex.unlock();
}

void Map::draw() {
	VTile& pos = player->position;
	VTile relativePos(
		pos.x - centerChunk.x * AbstractMeasures::TilesPerChunk - measures->getInnerWindowSizeTile().x / 2,
		pos.y - centerChunk.y * AbstractMeasures::TilesPerChunk - measures->getInnerWindowSizeTile().y / 2
	);
	mutex.lock();
	for (int i = 0; i < 2 * chunkRadius + 1; ++i)
		for (int j = 0; j < 2 * chunkRadius + 1; ++j)
			loaded[i][j]->draw(*renderWindow, relativePos + VTile(0.5, 0.5), VChunk(i, j) - VChunk(chunkRadius, chunkRadius));
	mutex.unlock();
}

void Map::update() {
	if (!shouldUpdate)
		return;
	VTile& pos = camera->getPosition();
	shouldUpdate = false;
	const VChunk newChunk(int(pos.x / AbstractMeasures::TilesPerChunk), int(pos.y / AbstractMeasures::TilesPerChunk), int(pos.z));
	const VChunk difference = newChunk - centerChunk;
	if (!difference.x && !difference.y && !difference.z)
		return;
	updateChunks(difference, newChunk);
	centerChunk = newChunk;
}

void Map::updateChunks(const VChunk& difference, const VChunk& tempCenter) {
	vector<vector<Chunk*>> newChunks(2 * chunkRadius + 1, vector<Chunk*>(2 * chunkRadius + 1, nullptr));
	vector<vector<bool>> reused(2 * chunkRadius + 1, vector<bool>(2 * chunkRadius + 1, false));
	for (int i = 0; i < 2 * chunkRadius + 1; ++i)
		for (int j = 0; j < 2 * chunkRadius + 1; ++j) {
			VChunk chunkPos = tempCenter + VChunk(i, j) - VChunk(chunkRadius, chunkRadius);
			if (i + difference.x >= 0 && i + difference.x < 2 * chunkRadius + 1 && j + difference.y >= 0 && j + difference.y < 2 * chunkRadius + 1) {
				newChunks[i][j] = loaded[i + difference.x][j + difference.y];
				reused[i + difference.x][j + difference.y] = true;
			}
			else
				newChunks[i][j] = new Chunk(chunkPos, provider);
		}
	mutex.lock();
	for (int i = 0; i < 2 * chunkRadius + 1; ++i)
		for (int j = 0; j < 2 * chunkRadius + 1; ++j)
			if (!reused[i][j])
				delete loaded[i][j];
	loaded = newChunks;
	mutex.unlock();
}

Tile* Map::getTileFromVTile(VTile tilePosition) {
	VChunk chunkOfTileClicked = VChunk(int(tilePosition.x / AbstractMeasures::TilesPerChunk), int(tilePosition.y / AbstractMeasures::TilesPerChunk));
	VChunk deltaChunkOffsetWithMiddleChunk = chunkOfTileClicked - centerChunk + VChunk(loaded.size() / 2, loaded.size() / 2);
	Tile* t = nullptr;
	if (deltaChunkOffsetWithMiddleChunk.x >= 0 && deltaChunkOffsetWithMiddleChunk.x < loaded.size() &&
		deltaChunkOffsetWithMiddleChunk.y >= 0 && deltaChunkOffsetWithMiddleChunk.y < loaded.size()) {
		Chunk* chunk = loaded[deltaChunkOffsetWithMiddleChunk.x][deltaChunkOffsetWithMiddleChunk.y];
		t = chunk->tiles[int(tilePosition.x - chunkOfTileClicked.x * AbstractMeasures::TilesPerChunk)][int(tilePosition.y - chunkOfTileClicked.y * AbstractMeasures::TilesPerChunk)];
	}
	return t;
}
