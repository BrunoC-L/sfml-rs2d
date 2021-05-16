#pragma once
#include "abstractService.h"
#include <string>

class AbstractResourceService : public AbstractService {
public:
	virtual void stop() = 0;
};
