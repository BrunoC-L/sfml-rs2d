#pragma once
#include "abstractService.h"

class AbstractTaskManager : public AbstractService {
public:
	vector<vector<function<bool()>>> tasksPerTick;
	virtual void executeAndRemove() = 0;
	virtual void scheduleTaskInTicks(function<bool()> task, unsigned ticksUntilCall) = 0;
};
