#pragma once
#include "../../common/common/abstractService.h"
#include "../../common/common/units.h"
#include <vector>
#include "chunk.h"

class AbstractMap : public AbstractService {
public:
	virtual void load() = 0;
	virtual Chunk* getChunk(VChunk v) = 0;
};
