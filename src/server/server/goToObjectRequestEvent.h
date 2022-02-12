#pragma once
#include "event.h"
#include "user.h"
#include "object.h"

struct GoToObjectRequestEventData {
	std::shared_ptr<User> user;
	Object* object;
	std::function<void()> callback;
};

using GoToObjectRequestEvent = Event<GoToObjectRequestEventData>;