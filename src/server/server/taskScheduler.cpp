#include "taskScheduler.h"
#include "tick.h"

TaskScheduler::TaskScheduler(ServiceProvider* provider) : Service(provider), AbstractTaskScheduler(provider) {
}

void TaskScheduler::callInTicks(int nTicks, std::function<void()> subscriber) {
	if (nTicks > subscribers.size())
		subscribers.resize(nTicks);
	subscribers[nTicks - 1].push_back(subscriber);
}

void TaskScheduler::init() {
	acquire();
	TickEvent::subscribe(
		new EventObserver<TickEvent>(
			[&](TickEvent* ev) {
				if (subscribers.size() == 0)
					return;
				for (auto& s : subscribers[0])
					s();
				subscribers.erase(subscribers.begin());
			}
			)
	);
}
