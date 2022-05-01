#pragma once
#include "event.h"
#include "user.h"
#include <memory>

struct LogoutEventData {
	std::shared_ptr<User> user;
};
using LogoutEvent = Event<LogoutEventData>;
