#pragma once
#include "abstractGameDataService.h"
#include "service.h"

class GameDataService : public Service, public AbstractGameDataService {
public:
	GameDataService(AbstractServiceProvider* provider, GameTickProgress* tracker, GameDataStorage* storage);
	virtual void init();

	virtual std::vector<VTile> getPlayerPositions();
	std::unordered_map<int, int> getPlayerPositionIndices();
};
