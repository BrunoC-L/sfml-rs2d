#pragma once
#include <vector>
#include <functional>

#include "NPC.h"
#include "groundItem.h"
#include "GameObject.h"
#include "../../common/units.h"
#include "abstractPlayer.h"
#include "mouseEvent.h"
#include "mouseMiddleClickEvent.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMoveEvent.h"
#include "teleportEvent.h"
#include "interactionClickEvent.h"
#include "walkClickEvent.h"
#include "rightClickTileEvent.h"

using namespace std;

class Tile {
public:
	Tile(int x, int y, int borders, const vector<GroundItem> groundItems, const vector<GameObject*> groundObjects, const vector<NPC> NPCs, const function<void()> callback);
	bool canMoveFrom(Tile from);
	bool canMove(VTile delta, int bordersTo, int bordersFrom);
	const int borders;
	const VTile position;
	virtual function<bool()> onLeftClick(MouseLeftClickEvent event);
	virtual function<bool()> onRightClick(MouseRightClickEvent event);
	virtual function<bool()> onMiddleClick(MouseMiddleClickEvent event);
	virtual function<bool()> onMove(MouseMoveEvent event);
	bool walkable;
	vector<GameObject*> gameObjects;
	vector<GroundObject> groundObjects;
	vector<GroundItem> groundItems;
	vector<NPC> NPCs;
	function<void()> callback;
};
