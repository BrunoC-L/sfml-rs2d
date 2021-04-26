#pragma once
#include "event.h"
#include "units.h"
#include <functional>

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#define TYPE InteractionClickEvent
#define PARENTS : public Event
#define MEMBERS std::function<bool(void)> f;
#define PARAMETERS std::function<bool(void)> f
#define MEMBERS_SET MEMBER_SET(f)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET)
