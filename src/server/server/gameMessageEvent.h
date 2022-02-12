#pragma once
#include "event.h"
#include "user.h"

struct GameMessageEventData {
	const std::shared_ptr<User>& user;
	const std::string& message;
};
using GameMessageEvent = Event<GameMessageEventData>;