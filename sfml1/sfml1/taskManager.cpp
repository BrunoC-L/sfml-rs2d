#include "taskManager.h"

//shared_ptr<TaskManager> singleton;
//
//shared_ptr<TaskManager> TaskManager::getInstance() {
//	if (!singleton)
//		singleton = shared_ptr<TaskManager>(new TaskManager());
//	return singleton;
//}

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
	cout << "in\n";
	while (tasksPerTick.size() < ticksUntilCall + 1)
		tasksPerTick.push_back({});
	cout << tasksPerTick.size() << endl << ticksUntilCall << endl;
	tasksPerTick[ticksUntilCall].push_back(task);
	cout << "out\n";
}