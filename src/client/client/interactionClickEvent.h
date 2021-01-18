#pragma once
#include "../../common/common/event.h"

#include "../../common/common/units.h"
#include <functional>
using namespace std;

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#define TYPE InteractionClickEvent
#define PARENTS : public Event
#define MEMBERS function<bool(void)> f;
#define PARAMETERS function<bool(void)> f
#define MEMBERS_SET MEMBER_SET(f)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET)
