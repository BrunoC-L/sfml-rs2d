#pragma once
#include "service.h"
#include "abstractPlayerActionService.h"
#include <vector>
#include "units.h"
#include "loginout.h"
#include "tick.h"

struct PathPosition {
	std::vector<VTile> path;
	VTile position;
};

class PlayerActionService : public AbstractPlayerActionService, public Service {
	std::unordered_map<std::shared_ptr<User>, PathPosition> pathPositions;
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
