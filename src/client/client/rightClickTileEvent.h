#pragma once
#include "event.h"

class Tile;

struct RightClickTileEventData {
    Tile* tile;
};
using RightClickTileEvent = Event<RightClickTileEventData>;
