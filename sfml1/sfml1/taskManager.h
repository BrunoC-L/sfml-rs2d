#pragma once
#include <functional>
#include <vector>
#include <memory>
#include <iostream>

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
	void scheduleTaskInTicks(function<void()> task, unsigned ticksUntilCall);
private:
	TaskManager() {
		tasksPerTick = vector<vector<function<void()>>>();
	};
	vector<vector<function<void()>>> tasksPerTick;
};
