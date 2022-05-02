#include "taskScheduler.h"

TaskScheduler::TaskScheduler(ServiceProvider* provider) : Service(provider), AbstractTaskScheduler(provider) {
	scheduleObserver.set([&](const ScheduleTaskEvent::Data& ev) {
		callInTicks(ev.nTicks, ev.task);
	});

	tickObserver.set([&](const TickEvent::Data& ev) {
		if (subscribers.size() == 0)
			return;
		for (auto& s : subscribers[0])
			s();
		subscribers.erase(subscribers.begin());
	});
}

void TaskScheduler::callInTicks(int nTicks, std::function<void()> subscriber) {
	if (nTicks > subscribers.size())
		subscribers.resize(nTicks);
	subscribers[nTicks - 1].push_back(subscriber);
}

void TaskScheduler::init() {
	acquire();
}
