#pragma once
#include "../../common/event.h"

#include "../../common/units.h"
#include "mouseEvent.h"
using namespace std;

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#define TYPE MouseMiddleClickEvent
#define PARENTS : public MouseEvent
#define MEMBERS 
#define PARAMETERS VPixel pos
#define MEMBERS_SET MEMBER_SET(pos)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS)