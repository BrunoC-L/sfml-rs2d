#include "GameTickProgress.h"
#include "constants.h"
#include <iostream>

ClockGameTickProgress::ClockGameTickProgress() {
	clock.restart();
}

void ClockGameTickProgress::onGameTick() {
	msBehind = msCurrentAnimWouldTakeAtNormalSpeed * (1 - getTickFraction());
	if (msBehind < 0)
		msBehind = 0;
	msCurrentAnimWouldTakeAtNormalSpeed = MSPT + msBehind;
	clock.restart();
	tick += 1;
}

double ClockGameTickProgress::getTickFraction() {
	auto tf = getMsSinceTick() / MSPT * (1 + msBehind / MSPT);
	return tf < 1 ? tf : 1;
}

double ClockGameTickProgress::getMsSinceTick() {
	return clock.getElapsedTime().asMilliseconds();
}

int ClockGameTickProgress::getTick() {
	return tick;
}
