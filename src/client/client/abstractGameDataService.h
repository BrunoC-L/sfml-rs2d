#pragma once
#include "abstractService.h"
#include "PlayerPositions.h"
#include <vector>

class AbstractGameDataService : public AbstractService {
public:
	virtual bool userIsLoggedIn() = 0;
	virtual std::vector<playerIdAndPosition> getPlayerPositions() = 0;
};
