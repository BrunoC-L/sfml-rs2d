#pragma once
#include "abstractMap.h"
#include "service.h"
#include <unordered_map>
#include <memory>

class Map : public AbstractMap, public Service {
	std::vector<std::shared_ptr<Chunk>> chunks;
public:
	Map(ServiceProvider* provider);
	virtual void init();
	virtual void load();
	virtual std::shared_ptr<Chunk> getChunk(VChunk v);
};
