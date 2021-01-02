#pragma once
#include "abstractServiceMacro.h"

SERVICES;

class AbstractTaskManager : public Service {
    SERVICE_MEMBERS;
	vector<vector<function<bool()>>> tasksPerTick;
	virtual void executeAndRemove() = 0;
	virtual void scheduleTaskInTicks(function<bool()> task, unsigned ticksUntilCall) = 0;
};
