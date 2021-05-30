#pragma once
#include "event.h"
#include "user.h"
#include <memory>

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITIALIZER_LIST
#define TYPE PlayerMoveEvent
#define PARENTS : public Event
#define MEMBERS const std::shared_ptr<const User>& user; const VTile& position;
#define PARAMETERS const std::shared_ptr<const User>& user, const VTile& position
#define MEMBERS_SET
#define MEMBERS_INITIALIZER_LIST : MEMBER_INITIALIZER(user), MEMBER_INITIALIZER(position)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET, MEMBERS_INITIALIZER_LIST)
