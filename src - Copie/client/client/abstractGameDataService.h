#pragma once
#include "abstractService.h"
#include "PlayerPositions.h"
#include "objectInteractions.h"
#include <vector>

class AbstractGameDataService : public AbstractService {
public:
	virtual const bool& userIsLoggedIn() = 0;
	virtual std::vector<playerIdAndPosition> getPlayerPositions() = 0;
	virtual std::pair<int*, std::vector<std::pair<VTile, ObjectInteractions>>> requestObjectsForChunk(VChunk chunk) = 0;
	virtual void clearObjectsCache() = 0;
};
