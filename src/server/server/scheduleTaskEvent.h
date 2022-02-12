#pragma once
#include "event.h"
#include <functional>

struct ScheduleTaskEventData {
	int nTicks;
	std::function<void()> task;
};
using ScheduleTaskEvent = Event<ScheduleTaskEventData>;
