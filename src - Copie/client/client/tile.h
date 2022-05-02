#pragma once
#include <vector>
#include <functional>
#include "NPC.h"
#include "units.h"
#include "abstractPlayer.h"
#include "mouseMiddleClickEvent.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMoveEvent.h"
#include "teleportEvent.h"
#include "interactionClickEvent.h"
#include "walkClickEvent.h"
#include "rightClickTileEvent.h"
#include "objectInteractions.h"

class Tile {
	std::vector<ObjectInteractions> interactions;
public:
	Tile(int x, int y);
	const VTile position;
	virtual void onLeftClick(MouseLeftClickEvent event);
	virtual void onRightClick(MouseRightClickEvent event);
	virtual void onMiddleClick(MouseMiddleClickEvent event);
	virtual const std::vector<ObjectInteractions>& getInteractions();
	virtual void addInteractions(ObjectInteractions oi);
};
