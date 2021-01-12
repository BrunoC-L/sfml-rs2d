#pragma once

class ClockTickScheduler {
	unsigned tickTime;
	sf::Clock clock;
public:
	ClockTickScheduler() : tickTime(0) {
		clock.restart();
	}

	bool shouldTick() {
		if (clock.getElapsedTime().asMilliseconds() < tickTime)
			return false;
		tickTime += 600;
		return true;
	}
};

class BoolTickScheduler {
public:
	bool ready;
	BoolTickScheduler() : ready(false) { }

	bool shouldTick() {
		if (!ready)
			return false;
		ready = false;
		return true;
	}
};

#include <mutex>
class MutexTickScheduler {
public:
	std::mutex m;
	MutexTickScheduler() {
		m.lock();
	}

	bool shouldTick() {
		m.lock();
		return true;
	}
};
