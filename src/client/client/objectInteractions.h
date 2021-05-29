#pragma once
#include "units.h"
#include <vector>
#include <string>

class ObjectInteractions {
private:
	std::vector<std::string> withExamine(std::vector<std::string> interactions) {
		interactions.push_back("Examine");
		return interactions;
	}
public:
	ObjectInteractions(VTile tile, std::string objectName, std::vector<std::string> interactions, int objectState) :
		tile(tile),
		objectName(objectName),
		interactions(withExamine(interactions)),
		objectState(objectState)
	{}
	VTile tile;
	std::string objectName;
	std::vector<std::string> interactions;
	int objectState;
};
