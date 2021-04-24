#pragma once
#include <SFML/System.hpp>

class GameTickProgress {
public:
	virtual double getTickFraction() = 0;
	virtual void onGameTick() = 0;
	virtual int getTick() = 0;
};

class ClockGameTickProgress : public GameTickProgress {
private:
	int tick = 0;
	double msBehind = 0;
	double msProgress = 0;
	double msCurrentAnimWouldTakeAtNormalSpeed = 0;
	sf::Clock clock;
	virtual double getMsSinceTick();
public:
	ClockGameTickProgress();
	virtual double getTickFraction() override;
	virtual void onGameTick() override;
	virtual int getTick() override;
};
