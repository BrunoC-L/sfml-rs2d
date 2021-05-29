#pragma once
#include "abstractService.h"
#include "packets.h"

class AbstractPlayerActionService : public AbstractService {
public:
	virtual const std::vector<std::vector<std::vector<std::shared_ptr<User>>>>& getUsersByChunk() = 0;
	virtual VTile getPlayerPosition(const std::shared_ptr<User>& user) = 0;
};
