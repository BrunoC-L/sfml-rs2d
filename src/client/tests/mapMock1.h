#pragma once
#include "map.h"

class MapMock : public Map {
public:
	MapMock(ServiceProvider* provider, int radius) : Map(provider, radius) {}
	void doUpdates() {}
	void load() {}
};
