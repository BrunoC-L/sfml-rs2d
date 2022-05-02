#pragma once
#include "event.h"
#include "user.h"
#include <memory>

struct PlayerPositionChangeEventData {
	std::shared_ptr<User> user;
	VTile newPosition;
	VTile oldPosition;
	VTile delta;
};
using PlayerPositionChangeEvent = Event<PlayerPositionChangeEventData>;
