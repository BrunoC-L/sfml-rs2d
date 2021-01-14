#pragma once
#include "../../common/common/event.h"

#include "../../common/common/units.h"

using namespace std;

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#define TYPE TeleportEvent
#define PARENTS
#define MEMBERS VTile pos;
#define PARAMETERS VTile pos
#define MEMBERS_SET MEMBER_SET(pos)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS)
