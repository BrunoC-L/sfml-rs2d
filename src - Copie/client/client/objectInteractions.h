#pragma once
#include "units.h"
#include <vector>
#include <string>

class ObjectInteractions {
public:
	ObjectInteractions(VTile tile, std::string objectName, std::vector<std::string> interactions, int objectState) :
		tile(tile),
		objectName(objectName),
		interactions(interactions),
		objectState(objectState)
	{}
	VTile tile;
	std::string objectName;
	std::vector<std::string> interactions;
	int objectState;
};
