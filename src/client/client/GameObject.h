#pragma once
#include "groundObject.h"
#include <vector>
#include "abstractMeasures.h"
#include "units.h"
#include "abstractPlayer.h"

class GameObject {
public:
	GameObject(int id, std::function<void(VTile, int)> updateObjectTexture);
	virtual std::vector<std::pair<std::string, std::function<bool()>>> getInteractions() = 0;
	virtual std::string getName() = 0;
	int id;
	std::function<void(VTile, int)> updateObjectTexture;
	std::vector<GroundObject> groundObjects;
	std::vector<std::pair<std::string, std::function<bool()>>> interactions;
	std::vector<VTile> positions;
	std::string name;
};
