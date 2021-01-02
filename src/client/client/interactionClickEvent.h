#pragma once
#include "event.h"

#include "units.h"
#include <functional>
using namespace std;

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#define TYPE InteractionClickEvent
#define PARENTS
#define MEMBERS function<bool(void)> f;
#define PARAMETERS function<bool(void)> f
#define MEMBERS_SET MEMBER_SET(f)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS)
