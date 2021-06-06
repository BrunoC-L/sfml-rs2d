#pragma once
#include "event.h"
#include "user.h"

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITIALIZER_LIST
#define TYPE GameMessageEvent
#define PARENTS : public Event
#define MEMBERS const std::shared_ptr<User>& user; const std::string& message;
#define PARAMETERS const std::shared_ptr<User>& user, const std::string& message
#define MEMBERS_SET
#define MEMBERS_INITIALIZER_LIST : MEMBER_INITIALIZER(user), MEMBER_INITIALIZER(message)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET, MEMBERS_INITIALIZER_LIST)
