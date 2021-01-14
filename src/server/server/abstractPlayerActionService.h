#pragma once
#include "../../common/common/abstractService.h"
#include "playerAction.h"
#include "user.h"
#include <memory>

class AbstractPlayerActionService : public AbstractService {
public:
	virtual void onGameTick() = 0;
};
