#include "map.h"

Map::Map(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("Map", this);
}

void Map::init() {
	acquire();
	load();
}

void Map::load() {
	chunks = vector<Chunk*>(24 * 28);
	for (int i = 17; i <= 19; ++i) { // i should be 0 to 28 in prod
		std::cout << "Loading chunks x = " << i << std::endl;
		for (int j = 12; j <= 14; ++j) {
			chunks[24 * i + j] = new Chunk(VChunk(i, j), provider);
		}
	}
}

Chunk* Map::getChunk(VChunk v) {
	// check if v is valid
	return chunks[24 * v.x + v.y];
}
