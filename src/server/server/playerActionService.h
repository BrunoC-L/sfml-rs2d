#pragma once
#include "service.h"
#include "abstractPlayerActionService.h"
#include <vector>
#include "units.h"
#include "loginout.h"
#include "tick.h"

class PlayerActionService : public AbstractPlayerActionService, public Service {
	std::unordered_map<std::shared_ptr<User>, std::vector<VTile>> paths;
	EventObserver<LogoutEvent> walkObserver;
	EventObserver<TickEvent> tickObserver;
public:
	PlayerActionService(ServiceProvider* provider);
	virtual void onGameTick();
	virtual void init();
	void sendPlayerPositions();
	void sendGameTick();
	virtual void stop() override;
};
