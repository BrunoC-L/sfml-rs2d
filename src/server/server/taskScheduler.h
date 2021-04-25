#pragma once
#include "abstractTaskScheduler.h"
#include "service.h"
#include <vector>

class TaskScheduler : public Service, public AbstractTaskScheduler {
private:
	std::vector<std::vector<std::function<void()>>> subscribers = {};
public:
	TaskScheduler(ServiceProvider* provider);
	virtual void callInTicks(int nTicks, std::function<void()> subscriber) override;
	virtual void init() override;
};
