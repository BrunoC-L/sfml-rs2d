#pragma once
#include <functional>
#include <vector>
#include "abstractServices.h"

using namespace std;

class TaskManager : public AbstractTaskManager {
	SERVICE_MEMBERS;
	TaskManager(AbstractServiceProvider* provider) {
		REGISTER(TaskManager);
		tasksPerTick = vector<vector<function<bool()>>>();
	}
	void init() {
		ACQUIRE;
	}
	void executeAndRemove();
	void scheduleTaskInTicks(function<bool()> task, unsigned ticksUntilCall);
};
