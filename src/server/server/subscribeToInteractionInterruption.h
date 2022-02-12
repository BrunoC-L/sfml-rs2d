#pragma once
#include "event.h"
#include "user.h"
#include <memory>
#include <functional>

struct SubscribeToInteractionInterruptionEventData {
	const std::shared_ptr<User>& user;
	std::function<void()> callback;
};
using SubscribeToInteractionInterruptionEvent = Event<SubscribeToInteractionInterruptionEventData>;