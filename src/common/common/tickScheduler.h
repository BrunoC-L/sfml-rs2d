#pragma once
#include <mutex>
#include <chrono>
#include <SFML/System.hpp>

class TickScheduler {
public:
	virtual bool shouldTick() = 0;
};

class ClockTickScheduler : public TickScheduler {
	int tickTime;
	sf::Clock clock;
public:
	ClockTickScheduler() : tickTime(600) {
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
		return shouldtick ^ (shouldtick = false);
	}
};
