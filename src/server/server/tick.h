#pragma once
#include "event.h"

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITALIZER_LIST
#define TYPE TickEvent
#define PARENTS : public Event
#define MEMBERS
#define PARAMETERS
#define MEMBERS_SET

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET)
