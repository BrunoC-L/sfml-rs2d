#include "random.h"

#ifdef __APPLE__
#define _ASSERT assert
#endif // __APPLE__

int randint(int a, int b) {
	_ASSERT(a < b);
	return rand() % (b - a) + a;
}

int randint(int a) {
	return rand() % a;
}

float randf(float a, float b) {
	float roll = (float(rand()) / float(RAND_MAX));
	return (b - a) * roll + a;
}

float randf(float a) {
	float roll = (float(rand()) / float(RAND_MAX));
	return roll * a;
}

bool rollPercent(float percent) {
	float roll = (float(rand()) / float(RAND_MAX)) * 100;
	return roll < percent;
}

bool rollChance(float chance) {
	float roll = float(rand()) / float(RAND_MAX);
	return roll < chance;
}
