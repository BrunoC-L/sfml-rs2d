#include "map.h"

Map::Map(ServiceProvider* provider) : Service(provider) {
	provider->set("Map", this);
}

void Map::init() {
	acquire();
	load();
}

void Map::load() {
	std::cout << "Loading Map";
	chunks = std::vector<std::shared_ptr<Chunk>>(29 * 25);
	for (int i = 0; i < 29; ++i) {
		std::cout << '.';
		for (int j = 0; j < 25; ++j) {
			chunks[24 * i + j] = std::make_shared<Chunk>(VChunk(i, j));
		}
	}
	std::cout << "\rLoading Map                                          \n"
				 "Done Loading Map\n";
}

std::shared_ptr<Chunk> Map::getChunk(VChunk v) {
	// check if v is valid
	return chunks[24 * v.x + v.y];
}
