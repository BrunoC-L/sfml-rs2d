#pragma once

#include "units.h"
#include "event.h"

#include "mouseEvent.h"
using namespace std;

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#define TYPE MouseWheelEvent
#define PARENTS : public MouseEvent
#define MEMBERS int delta;
#define PARAMETERS VPixel pos, int delta
#define MEMBERS_SET MEMBER_SET(pos) MEMBER_SET(delta)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET)
