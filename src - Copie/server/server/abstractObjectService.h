#pragma once
#include "abstractService.h"
#include <string>

class AbstractObjectService : public AbstractService {
public:
	virtual void interact(const std::shared_ptr<User>& user, VTile v, int objectState, const std::string& interaction, std::string objectName) = 0;
};
