#pragma once
#include <SFML/Graphics.hpp>
#include "groundObject.h"
#include "measures.h"
#include "units.h"

using namespace sf;
using namespace std;

class GameObject {
public:
	GameObject(int id, int* tilemapPtr, function<void(VTile, int)> updateObjectTexture);
	virtual vector<pair<string, function<bool()>>> getInteractions() = 0;
	int id;
	int* tilemapPtr;
	function<void(VTile, int)> updateObjectTexture;
	vector<GroundObject> groundObjects;
	bool hasRedClickAction;
	vector<pair<string, function<bool()>>> interactions;
};
