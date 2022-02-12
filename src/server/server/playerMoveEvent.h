#pragma once
#include "event.h"
#include "user.h"
#include <memory>

struct PlayerMoveEventData {
	const std::shared_ptr<const User>& user;
	const VTile& position;
};
using PlayerMoveEvent = Event<PlayerMoveEventData>;
