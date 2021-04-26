#pragma once
#include <vector>
#include <functional>
#include "NPC.h"
#include "groundItem.h"
#include "GameObject.h"
#include "units.h"
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

class Tile {
public:
	Tile(int x, int y);
	const VTile position;
	virtual void onLeftClick(MouseLeftClickEvent event);
	virtual void onRightClick(MouseRightClickEvent event);
	virtual void onMiddleClick(MouseMiddleClickEvent event);
};
