#pragma once
#include "../../common/abstractService.h"
#include "../../common/units.h"
#include <vector>

class AbstractGameDataService : public AbstractService {
public:
	std::vector<VTile> playerPositions;
};
