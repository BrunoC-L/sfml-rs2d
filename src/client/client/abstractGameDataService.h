#pragma once
#include "../../common/common/abstractService.h"
#include "../../common/common/units.h"
#include <vector>
#include "GameTickProgress.h"
#include "gameData.h"

class AbstractGameDataService : public AbstractService {
public:
	AbstractGameDataService(GameTickProgress* tracker, GameDataStorage* data) : tracker(tracker), data(data) {}
	GameTickProgress* tracker;
	GameDataStorage* data;
	virtual std::vector<VTile> getPlayerPositions() = 0;
};
