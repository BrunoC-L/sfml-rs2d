#pragma once
#include "abstractService.h"
#include "packets.h"

class AbstractPlayerActionService : public AbstractService {
public:
	virtual void walk(std::shared_ptr<User> user, WalkPacket& packet) = 0;
};
