#pragma once
#include "event.h"
#include "units.h"
#include <functional>

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#define TYPE WalkClickEvent
#define PARENTS : public Event
#define MEMBERS VTile pos;
#define PARAMETERS VTile pos
#define MEMBERS_SET MEMBER_SET(pos)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET)
