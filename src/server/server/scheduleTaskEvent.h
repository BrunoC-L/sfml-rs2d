#pragma once
#include "event.h"
#include <functional>

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITIALIZER_LIST
#define TYPE ScheduleTaskEvent
#define PARENTS : public Event
#define MEMBERS int nTicks; std::function<void()> task;
#define PARAMETERS int nTicks, std::function<void()> task
#define MEMBERS_SET
#define MEMBERS_INITIALIZER_LIST : MEMBER_INITIALIZER(nTicks), MEMBER_INITIALIZER(task)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET, MEMBERS_INITIALIZER_LIST)
