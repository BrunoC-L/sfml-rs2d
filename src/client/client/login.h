#pragma once
#include "event.h"
#include "json.h"

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITALIZER_LIST
#define TYPE LoginEvent
#define PARENTS : public Event
#define MEMBERS JSON json;
#define PARAMETERS JSON json
#define MEMBERS_SET MEMBER_SET(json)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET)

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITALIZER_LIST
#define TYPE LogoutEvent
#define PARENTS : public Event
#define MEMBERS
#define PARAMETERS
#define MEMBERS_SET

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET)
