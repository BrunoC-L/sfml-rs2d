#pragma once
#include "abstractGameDataService.h"
#include "service.h"

class GameDataService : public Service, public AbstractGameDataService {
public:
	GameDataService(AbstractServiceProvider* provider, GameTickProgress* tracker, GameDataStorage* data);
	virtual void init();

	virtual std::vector<VTile> getPlayerPositions();
};
