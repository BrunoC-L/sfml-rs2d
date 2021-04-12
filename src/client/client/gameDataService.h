#pragma once
#include "abstractGameDataService.h"
#include "GameTickProgress.h"
#include "service.h"
#include "PlayerPositions.h"

class GameDataService : public Service, public AbstractGameDataService {
	bool loggedIn = false;
	GameTickProgress* tracker;
	PlayerPositions playerPositions;
	void storePositions(JSON& json);
public:
	GameDataService(AbstractServiceProvider* provider, GameTickProgress* tracker);
	virtual void init();

	virtual bool userIsLoggedIn() override;
	virtual std::vector<playerIdAndPosition> getPlayerPositions() override;
};
