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
	msCurrentAnimWouldTakeAtNormalSpeed = mspt + msBehind;
	clock.restart();
	tick += 1;
}

double ClockGameTickProgress::getTickFraction() {
	auto tf = getMsSinceTick() / mspt * (1 + msBehind / mspt);
	return tf;
}

double ClockGameTickProgress::getMsSinceTick() {
	return clock.getElapsedTime().asMilliseconds();
}

int ClockGameTickProgress::getTick() {
	return tick;
}
