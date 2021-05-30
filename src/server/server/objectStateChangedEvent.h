#pragma once
#include "event.h"

class Object;

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITIALIZER_LIST
#define TYPE ObjectStateChangedEvent
#define PARENTS : public Event
#define MEMBERS Object* object;
#define PARAMETERS Object* object
#define MEMBERS_SET MEMBER_SET(object)
#define MEMBERS_INITIALIZER_LIST

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET, MEMBERS_INITIALIZER_LIST)
