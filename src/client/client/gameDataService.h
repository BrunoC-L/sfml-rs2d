#pragma once
#include "abstractGameDataService.h"
#include "GameTickProgress.h"
#include "service.h"
#include "PlayerPositions.h"
#include "login.h"

class GameDataService : public Service, public AbstractGameDataService {
	bool loggedIn = false;
	GameTickProgress* tracker;
	std::unique_ptr<PlayerPositions> playerPositions;
	EventObserver<LoginEvent> loginObserver;
	EventObserver<LogoutEvent> logoutObserver;
	void storePositions(JSON& json);
public:
	GameDataService(ServiceProvider* provider, GameTickProgress* tracker);
	virtual void init();

	virtual const bool& userIsLoggedIn() override;
	virtual std::vector<playerIdAndPosition> getPlayerPositions() override;
};
