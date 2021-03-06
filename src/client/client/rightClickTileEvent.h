#pragma once
#include "event.h"

class Tile;

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITALIZER_LIST
#define TYPE RightClickTileEvent
#define PARENTS : public Event
#define MEMBERS Tile* tile;
#define PARAMETERS Tile* tile
#define MEMBERS_SET MEMBER_SET(tile)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET)
