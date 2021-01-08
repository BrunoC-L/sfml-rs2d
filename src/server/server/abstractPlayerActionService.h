#pragma once
#include "../../common/abstractService.h"
#include "playerAction.h"
#include "user.h"
#include <memory>

class AbstractPlayerActionService : public AbstractService {
public:
	virtual void onGameTick() = 0;
	virtual void onAction(std::shared_ptr<PlayerAction> action) = 0;
};
