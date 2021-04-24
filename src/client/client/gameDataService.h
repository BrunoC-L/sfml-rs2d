#pragma once
#include "abstractGameDataService.h"
#include "GameTickProgress.h"
#include "service.h"
#include "PlayerPositions.h"

class GameDataService : public Service, public AbstractGameDataService {
	bool loggedIn = false;
	GameTickProgress* tracker;
	std::unique_ptr<PlayerPositions> playerPositions;
	void storePositions(JSON& json);
public:
	GameDataService(ServiceProvider* provider, GameTickProgress* tracker);
	virtual void init();

	virtual const bool& userIsLoggedIn() override;
	virtual std::vector<playerIdAndPosition> getPlayerPositions() override;
};
