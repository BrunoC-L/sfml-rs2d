#include "map.h"

Map::Map(ServiceProvider* provider) : Service(provider) {
	provider->set("Map", this);
}

void Map::init() {
	acquire();
	load();
}

void Map::load() {
	chunks = std::vector<std::shared_ptr<Chunk>>(24 * 28);
	for (int i = 17; i <= 19; ++i) { // i should be 0 to 28 in prod
		std::cout << "Loading chunks x = " << i << std::endl;
		for (int j = 12; j <= 14; ++j) {
			chunks[24 * i + j] = std::make_shared<Chunk>(VChunk(i, j));
		}
	}
}

std::shared_ptr<Chunk> Map::getChunk(VChunk v) {
	// check if v is valid
	return chunks[24 * v.x + v.y];
}
