#pragma once
#include "abstractService.h"
#include "serviceProvider.h"
#include <functional>
#include "services.h"

class AbstractTaskScheduler : public AbstractService {
public:
	AbstractTaskScheduler(ServiceProvider* provider) {
		provider->set(SCHEDULER, this);
	}
	virtual void callInTicks(int nTicks, std::function<void()> subscriber) = 0;
	virtual void stop() = 0;
};
