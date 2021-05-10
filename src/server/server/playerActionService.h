#pragma once
#include "service.h"
#include "abstractPlayerActionService.h"
#include <vector>
#include "units.h"
#include "loginEvent.h"
#include "logoutEvent.h"
#include "tickEvent.h"
#include "constants.h"

struct PathPosition {
	std::vector<VTile> path;
	VTile position;
};

class PlayerActionService : public AbstractPlayerActionService, public Service {
	PathPosition pathPositions[MAX_PLAYERS_ONLINE];
	std::vector<std::pair<std::shared_ptr<User>, VTile>> positions[29][25];
	EventObserver<LogoutEvent> logoutObserver;
	EventObserver<LoginEvent> loginObserver;
	EventObserver<TickEvent> tickObserver;
	void updatePlayerPositions();
	void sendPlayerPositions();
	void sendGameTick();
public:
	PlayerActionService(ServiceProvider* provider);
	virtual void onGameTick();
	virtual void init();
	virtual void stop() override;
};
