#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

#include "../../common/common/units.h"
//#include "NPC.h"
//#include "groundItem.h"
//#include "GameObject.h"
//#include "abstractPlayer.h"
//#include "mouseEvent.h"
//#include "teleportEvent.h"
//#include "interactionClickEvent.h"
//#include "walkClickEvent.h"
//#include "rightClickTileEvent.h"

using namespace std;

class Tile {
public:
	Tile(int x, int y, int borders);
	bool canMoveFrom(Tile from);
	bool canMove(VTile delta, int bordersTo, int bordersFrom);
	const int borders;
	const VTile position;
	bool walkable;
	//vector<GameObject*> gameObjects;
	//vector<GroundObject> groundObjects;
	//vector<GroundItem> groundItems;
	//vector<NPC> NPCs;
};
