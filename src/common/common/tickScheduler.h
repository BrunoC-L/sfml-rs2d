#pragma once
#include <mutex>
#include <chrono>
#include <SFML/System.hpp>
#include "constants.h"

class TickScheduler {
public:
	virtual bool shouldTick() = 0;
};

class ClockTickScheduler : public TickScheduler {
	int tickTime;
	sf::Clock clock;
public:
	ClockTickScheduler() : tickTime(MSPT) {
		clock.restart();
	}

	bool shouldTick() {
		auto tms = clock.getElapsedTime().asMilliseconds();
		auto tsleep = tickTime - tms;
		if (tsleep > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(tsleep));
		clock.restart();
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
