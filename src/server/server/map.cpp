#include "map.h"
#include "clamp.h"

Map::Map(ServiceProvider* provider, ObjectFactory& objectFactory) : Service(provider), objectFactory(objectFactory) {
	provider->set(MAP, this);
}

void Map::init() {
	acquire();
	load();
}

void Map::load() {
	chunks = std::vector<std::shared_ptr<Chunk>>(29 * 25);
	for (int i = 0; i < 29; ++i) {
		std::cout << "\rLoading Map " << i << "/29 \b";
		for (int j = 0; j < 25; ++j) {
			chunks[25 * i + j] = std::make_shared<Chunk>(VChunk(i, j), objectFactory);
		}
	}
	std::cout << "\r                         \r";
}

std::shared_ptr<Chunk>& Map::getChunk(VChunk v) {
	return chunks[25 * v.x + v.y];
}

Tile* Map::getTile(VTile v) {
	if (clamp(v.x, 0, 29 * TilesPerChunk - 1) && clamp(v.y, 0, 25 * TilesPerChunk - 1))
		return getChunk(VChunk(int(v.x / TilesPerChunk), int(v.y / TilesPerChunk)))->tiles[int(v.x) % int(TilesPerChunk)][int(v.y) % int(TilesPerChunk)];
	return nullptr;
}
