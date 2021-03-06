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
#include "subscribeToInteractionInterruption.h"

struct PathPosition {
	std::vector<VTile> path;
	VTile position;
};

class PlayerActionService : public AbstractPlayerActionService, public Service {
	PathPosition pathPositions[MAX_PLAYERS_ONLINE];
	VTile oldPositions[MAX_PLAYERS_ONLINE];
	std::function<void()> movementCompleteCallbacks[MAX_PLAYERS_ONLINE];
	std::function<void()> interactionInterruptionCallbacks[MAX_PLAYERS_ONLINE];
	std::vector<std::pair<std::shared_ptr<User>, VTile>> positions[29][25];
	std::vector<std::vector<std::vector<std::shared_ptr<User>>>> usersByChunk;

	EventObserver<LoginEvent> loginObserver;
	EventObserver<LogoutEvent> logoutObserver;
	EventObserver<TickEvent> tickObserver;
	EventObserver<GoToObjectRequest> goToObjectObserver;
	EventObserver<SubscribeToInteractionInterruptionEvent> interruptionSubscriptionObserver;

	void updatePlayerPositions();
	void sendPlayerPositions();
	void sendGameTick();
	void onGameTick();
	void checkForTileAndChunkChanges();
	void walk(std::shared_ptr<User> user, std::vector<VTile> destination);
	void walk(std::shared_ptr<User> user, WalkPacket& packet);
public:
	PlayerActionService(ServiceProvider* provider);
	virtual void init() override;
	virtual VTile getPlayerPosition(const std::shared_ptr<User>& user) override;
};
