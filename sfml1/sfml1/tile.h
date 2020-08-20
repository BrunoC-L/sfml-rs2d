#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

#include "NPC.h"
#include "groundItem.h"
#include "GameObject.h"
#include "units.h"
#include "player.h"
#include "rightClickInterface.h"
#include "mouseEvent.h"
#include "MouseEventVisitor.h"

using namespace std;
using namespace sf;

class Tile : public MouseEventVisitor {
public:
	Tile(int x, int y, int borders, const vector<GroundItem> groundItems, const vector<GameObject*> groundObjects, const vector<NPC> NPCs, const function<void()> callback);
	bool canMoveFrom(Tile from);
	const int borders;
	const VTile position;
	virtual function<bool()> onLeftClick(MouseLeftClickEvent event);
	virtual function<bool()> onRightClick(MouseRightClickEvent event);
	virtual function<bool()> onMiddleClick(MouseMiddleClickEvent event);
	virtual function<bool()> onMove(MouseMoveEvent event);
	bool walkable;
private:
	vector<GameObject*> gameObjects;
	vector<GroundObject> groundObjects;
	vector<GroundItem> groundItems;
	vector<NPC> NPCs;
	function<void()> callback;
};
