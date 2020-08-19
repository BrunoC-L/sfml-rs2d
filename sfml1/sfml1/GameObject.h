#pragma once
#include <SFML/Graphics.hpp>
#include "groundObject.h"
#include "measures.h"
#include "units.h"
#include "player.h"
#include "gameTick.h"

using namespace sf;
using namespace std;

class GameObject {
public:
	GameObject(int id, function<void(VTile, int)> updateObjectTexture);
	virtual vector<pair<string, function<bool()>>> getInteractions() = 0;
	int id;
	function<void(VTile, int)> updateObjectTexture;
	vector<GroundObject> groundObjects;
	bool hasRedClickAction;
	vector<pair<string, function<bool()>>> interactions;
	vector<VTile> positions;
	string name;
	virtual string getName();
};
