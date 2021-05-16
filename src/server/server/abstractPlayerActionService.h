#pragma once
#include "abstractService.h"

class AbstractPlayerActionService : public AbstractService {
public:
	virtual void stop() = 0;
};
