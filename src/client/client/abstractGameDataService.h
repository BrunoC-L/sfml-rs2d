#pragma once
#include "../../common/common/abstractService.h"
#include "../../common/common/units.h"
#include <vector>

class AbstractGameDataService : public AbstractService {
public:
	virtual std::vector<VTile> getPlayerPositions() = 0;
	virtual void updatePlayerPosition() = 0;
};
