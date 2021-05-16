#include "map.h"

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
			chunks[24 * i + j] = std::make_shared<Chunk>(VChunk(i, j), objectFactory);
		}
	}
	std::cout << "\r                         \r";
}

std::shared_ptr<Chunk> Map::getChunk(VChunk v) {
	return chunks[24 * v.x + v.y];
}

void Map::stop() {
	
}
