#pragma once
#include "event.h"

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITIALIZER_LIST
#define TYPE TickEvent
#define PARENTS : public Event
#define MEMBERS
#define PARAMETERS
#define MEMBERS_SET
#define MEMBERS_INITIALIZER_LIST

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET, MEMBERS_INITIALIZER_LIST)
