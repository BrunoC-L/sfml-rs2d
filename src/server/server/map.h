#pragma once
#include "service.h"
#include <unordered_map>
#include <memory>
#include "service.h"
#include "units.h"
#include <vector>
#include "chunk.h"

class Map : public Service {
	std::vector<std::shared_ptr<Chunk>> chunks;
	ObjectFactory& objectFactory;
public:
	Map(ServiceProvider* provider, ObjectFactory& objectFactory);
	virtual void init();
	virtual void load();
	virtual std::shared_ptr<Chunk>& getChunk(VChunk v);
	virtual Tile* getTile(VTile v);
};
