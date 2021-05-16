#pragma once
#include "service.h"
#include "abstractResourceService.h"

class ResourceService : public AbstractResourceService, public Service {
public:
	virtual void init() override;
	virtual void stop() override;
};
