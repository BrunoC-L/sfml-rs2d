#include "taskManager.h"

void TaskManager::executeAndRemove() {
	if (!tasksPerTick.size())
		return;
	for (auto task : tasksPerTick[0]) {
		try {
			task();
		}
		catch (...) { }
	}
	tasksPerTick.erase(tasksPerTick.begin());
}

void TaskManager::scheduleTaskInTicks(function<void()> task, unsigned ticksUntilCall) {
	while (tasksPerTick.size() < ticksUntilCall + 1)
		tasksPerTick.push_back({});
	tasksPerTick[ticksUntilCall].push_back(task);
}