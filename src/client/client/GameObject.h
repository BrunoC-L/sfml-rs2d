#pragma once
#include "groundObject.h"
#include <vector>
#include "abstractMeasures.h"
#include "units.h"
#include "abstractPlayer.h"

using namespace std;

class GameObject {
public:
	GameObject(int id, function<void(VTile, int)> updateObjectTexture);
	virtual vector<pair<string, function<bool()>>> getInteractions() = 0;
	virtual string getName() = 0;
	int id;
	function<void(VTile, int)> updateObjectTexture;
	vector<GroundObject> groundObjects;
	vector<pair<string, function<bool()>>> interactions;
	vector<VTile> positions;
	string name;
};
