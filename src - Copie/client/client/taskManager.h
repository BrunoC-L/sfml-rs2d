#pragma once
#include <functional>
#include <vector>
#include "abstractServices.h"
#include "service.h"

using namespace std;

class TaskManager : public AbstractTaskManager, public Service {
public:
	TaskManager(AbstractServiceProvider* provider) : Service(provider) {
		provider->set("TaskManager", this);
		tasksPerTick = vector<vector<function<bool()>>>();
	}
	virtual void init() {
		acquire();
	}
	virtual void executeAndRemove();
	virtual void scheduleTaskInTicks(function<bool()> task, unsigned ticksUntilCall);
};
