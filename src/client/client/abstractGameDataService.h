#pragma once
#include "abstractService.h"
#include "PlayerPositions.h"
#include <vector>

class AbstractGameDataService : public AbstractService {
public:
	virtual const bool& userIsLoggedIn() = 0;
	virtual std::vector<playerIdAndPosition> getPlayerPositions() = 0;
};
