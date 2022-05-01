#pragma once
#include "abstractMap.h"
#include "service.h"
#include <unordered_map>
#include <memory>

class Map : public AbstractMap, public Service {
	std::vector<std::shared_ptr<Chunk>> chunks;
	ObjectFactory& objectFactory;
public:
	Map(ServiceProvider* provider, ObjectFactory& objectFactory);
	virtual void init() override;
	virtual void load() override;
	virtual std::shared_ptr<Chunk>& getChunk(VChunk v) override;
	virtual Tile* getTile(VTile v) override;
};
