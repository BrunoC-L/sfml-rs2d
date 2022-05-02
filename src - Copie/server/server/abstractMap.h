#pragma once
#include "abstractService.h"
#include "units.h"
#include <vector>
#include "chunk.h"

class AbstractMap : public AbstractService {
public:
	virtual void load() = 0;
	virtual std::shared_ptr<Chunk>& getChunk(VChunk v) = 0;
	virtual Tile* getTile(VTile v) = 0;
};
