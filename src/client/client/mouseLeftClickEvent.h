#pragma once
#include "event.h"
#include "units.h"
#include "mouseEvent.h"

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#define TYPE MouseLeftClickEvent
#define PARENTS : public MouseEvent
#define MEMBERS 
#define PARAMETERS VPixel pos
#define MEMBERS_SET MEMBER_SET(pos)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET)
