#pragma once
#include "map.h"

class MapMock : public Map {
public:
	MapMock(ServiceProvider* provider) : Map(provider) {}
	void doUpdates() {}
	void load() {}
};
