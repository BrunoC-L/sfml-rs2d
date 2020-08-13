#pragma once
#include <functional>
#include <vector>

using namespace std;

class TaskManager {
public:
	TaskManager(const TaskManager& other) = delete;
	TaskManager operator=(const TaskManager& other) = delete;
	static TaskManager& getInstance() {
		static TaskManager instance;
		return instance;
	}
	void executeAndRemove();
	void scheduleTaskInTicks(function<bool()> task, unsigned ticksUntilCall);
private:
	TaskManager() {
		tasksPerTick = vector<vector<function<bool()>>>();
	};
	vector<vector<function<bool()>>> tasksPerTick;
};
