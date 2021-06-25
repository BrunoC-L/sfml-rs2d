#pragma once
#include "service.h"
#include <vector>
#include "tickEvent.h"
#include "scheduleTaskEvent.h"
#include <functional>

class TaskScheduler : public Service {
private:
	std::vector<std::vector<std::function<void()>>> subscribers = {};
	EventObserver<TickEvent> tickObserver;
	EventObserver<ScheduleTaskEvent> scheduleObserver;
public:
	TaskScheduler(ServiceProvider* provider);
	virtual void callInTicks(int nTicks, std::function<void()> subscriber);
	virtual void init();
};
