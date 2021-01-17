#pragma once
#include <SFML/System.hpp>

class GameTickProgress {
public:
	virtual double getTickFraction() = 0;
	virtual void onGameTick() = 0;
};

class ClockGameTickProgress : public GameTickProgress {
public:
	double msBehind = 0;
	double msProgress = 0;
	double msCurrentAnimWouldTakeAtNormalSpeed = 0;
	sf::Clock clock;
	ClockGameTickProgress();
	virtual double getTickFraction();
	virtual void onGameTick();
	virtual double getMsSinceTick();
};
