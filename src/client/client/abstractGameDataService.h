#pragma once
#include "../../common/common/abstractService.h"
#include "../../common/common/units.h"
#include <vector>

class AbstractGameDataService : public AbstractService {
public:
	std::vector<VTile> playerPositions;
};
