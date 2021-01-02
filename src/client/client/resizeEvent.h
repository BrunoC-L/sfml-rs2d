#pragma once
#include "event.h"

class Tile;

using namespace std;

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITALIZER_LIST
#define TYPE ResizeEvent
#define PARENTS
#define MEMBERS
#define PARAMETERS
#define MEMBERS_SET

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS)
