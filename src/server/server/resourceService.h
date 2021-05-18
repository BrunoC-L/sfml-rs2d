#pragma once
#include "service.h"
#include "abstractResourceService.h"

class ResourceService : public AbstractResourceService, public Service {
public:
	ResourceService(ServiceProvider* provider);
	virtual void init() override;
	virtual void interact(const std::shared_ptr<User>& user, VTile v, int objectState, int interactionIndex, int objectId) override;
};
