#pragma once
#include "event.h"
#include "user.h"
#include <memory>

struct LoginEventData {
	std::shared_ptr<User> user;
	VTile position;
};
using LoginEvent = Event<LoginEventData>;