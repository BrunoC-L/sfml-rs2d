#pragma once
#include "service.h"
#include "abstractPlayerActionService.h"
#include <vector>
#include "../../common/units.h"

class PlayerActionService : public AbstractPlayerActionService, public Service {
	std::vector<std::shared_ptr<PlayerAction>> actions;
	std::unordered_map<std::shared_ptr<User>, std::vector<VTile>> paths;
public:
	PlayerActionService(AbstractServiceProvider* provider);
	virtual void onGameTick();
	virtual void onAction(std::shared_ptr<PlayerAction> action);
	virtual void init();
};
