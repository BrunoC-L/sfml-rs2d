#pragma once
#include <mutex>
#include <chrono>
#include "constants.h"

class TickScheduler {
public:
	virtual bool shouldTick() = 0;
};

class ClockTickScheduler : public TickScheduler {
	int tickTime;
	std::chrono::steady_clock::time_point begin;
public:
	ClockTickScheduler() : tickTime(MSPT), begin(std::chrono::steady_clock::now()) { }

	bool shouldTick() {
		std::chrono::steady_clock::time_point now(std::chrono::steady_clock::now());
		auto delta = now - begin;
		begin = now;
		int tms = std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();
		int tsleep = tickTime - tms;
		if (tsleep > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(tsleep));
		return true;
	}
};

class BoolTickScheduler : public TickScheduler {
public:
	bool shouldtick;
	BoolTickScheduler() : shouldtick(false) { }

	bool shouldTick() {
        auto previous = shouldtick;
        shouldtick = false;
		return shouldtick ^ previous;
	}
};
