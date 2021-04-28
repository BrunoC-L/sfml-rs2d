#pragma once
#include "abstractService.h"
#include "playerAction.h"
#include "user.h"
#include <memory>

class AbstractPlayerActionService : public AbstractService {
public:
	virtual void stop() = 0;
};
