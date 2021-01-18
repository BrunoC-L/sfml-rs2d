#pragma once
#include "../../common/common/abstractService.h"
#include "../../common/common/units.h"
#include <vector>
#include "GameTickProgress.h"
#include "gameData.h"

class AbstractGameDataService : public AbstractService {
public:
	bool userIsLoggedIn = false;
	AbstractGameDataService(GameTickProgress* tracker, GameDataStorage* storage) : tracker(tracker), storage(storage) {}
	GameTickProgress* tracker;
	GameDataStorage* storage;
	virtual std::vector<VTile> getPlayerPositions() = 0;
};
