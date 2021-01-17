#pragma once
#include "abstractGameDataService.h"
#include "service.h"
#include "GameTickProgress.h"
#include "gameData.h"

class GameDataService : public Service, public AbstractGameDataService {
	GameTickProgress* tracker;
	GameDataStorage* data;
public:
	GameDataService(AbstractServiceProvider* provider, GameTickProgress* tracker, GameDataStorage* data);
	virtual void init();

	virtual std::vector<VTile> getPlayerPositions();
	virtual void updatePlayerPosition();
};
