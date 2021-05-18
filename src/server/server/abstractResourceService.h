#pragma once
#include "abstractService.h"
#include <string>

class AbstractResourceService : public AbstractService {
public:
	virtual void interact(const std::shared_ptr<User>& user, VTile v, int objectState, int interactionIndex, int objectId) = 0;
};
