#pragma once
#include "event.h"
#include "user.h"
#include <memory>


#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITALIZER_LIST
#define TYPE LogoutEvent
#define PARENTS : public Event
#define MEMBERS std::shared_ptr<User> user;
#define PARAMETERS std::shared_ptr<User> user
#define MEMBERS_SET MEMBER_SET(user)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET)
