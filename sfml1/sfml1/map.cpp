#include "map.h"
#include <iostream>

Map::Map() {
	VTile& pos = Camera::getPosition();
	centerChunk = VChunk(int(pos.x / Measures::TilesPerChunk), int(pos.y / Measures::TilesPerChunk), int(pos.z));
}

void Map::load() {
	Measures& measures = Measures::getInstance();
	Textures& textures = Textures::getInstance();
	loaded = vector<vector<Chunk*>>(2 * chunkRadius + 1, vector<Chunk*>(2 * chunkRadius + 1, nullptr));
	mutex.lock();
	for (int i = 0; i < 2 * chunkRadius + 1; ++i)
		for (int j = 0; j < 2 * chunkRadius + 1; ++j) {
			VChunk chunkPos = centerChunk + VChunk(i, j) - VChunk(chunkRadius, chunkRadius);
			delete loaded[i][j];
			loaded[i][j] = new Chunk(chunkPos, measures, textures);
		}
	mutex.unlock();
}

void Map::draw() {
	Measures& measures = Measures::getInstance();
	RenderWindow& w = RenderWindowSingleton::getInstance();
	VTile& pos = Player::getInstance().position;
	VTile relativePos(pos.x - centerChunk.x * Measures::TilesPerChunk - measures.getInnerWindowSizeTile().x / 2, pos.y - centerChunk.y * Measures::TilesPerChunk - measures.getInnerWindowSizeTile().y / 2);
	mutex.lock();
	for (int i = 0; i < 2 * chunkRadius + 1; ++i)
		for (int j = 0; j < 2 * chunkRadius + 1; ++j)
			loaded[i][j]->draw(w, relativePos + VTile(0.5, 0.5), VChunk(i, j) - VChunk(chunkRadius, chunkRadius));
	mutex.unlock();
}

void Map::update() {
	if (!shouldUpdate)
		return;
	VTile& pos = Camera::getPosition();
	shouldUpdate = false;
	const VChunk newChunk(int(pos.x / Measures::TilesPerChunk), int(pos.y / Measures::TilesPerChunk), int(pos.z));
	const VChunk difference = newChunk - centerChunk;
	if (difference == VChunk(0, 0, 0))
		return;
	updateChunks(difference, newChunk);
	centerChunk = newChunk;
}

void Map::updateChunks(const VChunk& difference, const VChunk& tempCenter) {
	Measures& measures = Measures::getInstance();
	Textures& textures = Textures::getInstance();
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
				newChunks[i][j] = new Chunk(chunkPos, measures, textures);
		}
	mutex.lock();
	for (int i = 0; i < 2 * chunkRadius + 1; ++i)
		for (int j = 0; j < 2 * chunkRadius + 1; ++j)
			if (!reused[i][j])
				delete loaded[i][j];
	loaded = newChunks;
	mutex.unlock();
}
