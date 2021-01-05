#pragma once
#include "../../common/abstractService.h"
#include "../../common/units.h"
#include <vector>
#include "chunk.h"

class AbstractMap : public AbstractService {
public:
	virtual void load() = 0;
	virtual Chunk* getChunk(VChunk v) = 0;
};
