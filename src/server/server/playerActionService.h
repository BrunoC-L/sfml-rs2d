#pragma once
#include "service.h"
#include "abstractPlayerActionService.h"
#include <vector>
#include "units.h"

class PlayerActionService : public AbstractPlayerActionService, public Service {
	std::unordered_map<std::shared_ptr<User>, std::vector<VTile>> paths;
public:
	PlayerActionService(AbstractServiceProvider* provider);
	virtual void onGameTick();
	virtual void init();
	void sendPlayerPositions();
	void sendGameTick();
};
