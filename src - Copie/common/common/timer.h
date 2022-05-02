#pragma once
#include <SFML/System.hpp>

class Time {
	unsigned time;
public:
	Time() = default;
	Time(unsigned us): time(us) { }
	unsigned us() { return time; }
	unsigned ms() { return time / 1000; }
	unsigned s()  { return time / 1000000; }
};

class Timer {
public:
	virtual Time get() = 0;
	virtual void restart() = 0;
};

class clockTimer {
	sf::Clock clock;
public:
	virtual Time get() {
		return Time(clock.getElapsedTime().asMicroseconds());
	}
	virtual void restart() {
		clock.restart();
	}
};

class VarTimer {
public:
	Time t;
	virtual Time get() {
		return t;
	}
	virtual void restart() {
		t = Time();
	}
};
