#pragma once
#include "abstractService.h"
#include "packets.h"

class AbstractPlayerActionService : public AbstractService {
public:
	virtual VTile getPlayerPosition(const std::shared_ptr<User>& user) = 0;
};
