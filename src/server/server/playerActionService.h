#pragma once
#include "service.h"
#include "abstractPlayerActionService.h"
#include <vector>
#include "units.h"
#include "loginEvent.h"
#include "logoutEvent.h"
#include "tickEvent.h"
#include "constants.h"
#include "playerMoveEvent.h"
#include "goToObjectRequestEvent.h"
#include <set>

struct PathPosition {
	std::vector<VTile> path;
	VTile position;
};

class PlayerActionService : public AbstractPlayerActionService, public Service {
	PathPosition pathPositions[MAX_PLAYERS_ONLINE];
	std::shared_ptr<std::function<void()>> movementCompleteCallbacks[MAX_PLAYERS_ONLINE];
	std::vector<std::pair<std::shared_ptr<User>, VTile>> positions[29][25];
	EventObserver<LogoutEvent> logoutObserver;
	EventObserver<LoginEvent> loginObserver;
	EventObserver<TickEvent> tickObserver;
	EventObserver<GoToObjectRequest> goToObjectObserver;
	void updatePlayerPositions();
	void sendPlayerPositions();
	void sendGameTick();
	void onGameTick();
	void walk(std::shared_ptr<User> user, std::vector<VTile> destination);
public:
	PlayerActionService(ServiceProvider* provider);
	virtual void init() override;
	virtual void walk(std::shared_ptr<User> user, WalkPacket& packet) override;
};
