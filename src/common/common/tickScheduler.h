#pragma once
#include <mutex>

class TickScheduler {
public:
	virtual bool shouldTick() = 0;
};

class ClockTickScheduler : public TickScheduler {
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

class BoolTickScheduler : public TickScheduler {
public:
	bool shouldtick;
	BoolTickScheduler() : shouldtick(false) { }

	bool shouldTick() {
		return shouldtick ^ (shouldtick = false);
		//if (!shouldtick)
		//	return false;
		//shouldtick = false;
		//return true;
	}
};

class MutexTickScheduler : public TickScheduler {
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
