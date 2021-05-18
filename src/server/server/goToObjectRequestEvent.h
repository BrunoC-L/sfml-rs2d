#pragma once
#include "event.h"
#include "user.h"
#include "object.h"
#include "requestEvent.h"

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITALIZER_LIST
#define TYPE GoToObjectRequest
#define PARENTS : public RequestEvent<void>
#define MEMBERS std::shared_ptr<User> user; Object* object; std::function<void()> callback;
#define PARAMETERS std::shared_ptr<User> user, Object* object, std::function<void()> callback
#define MEMBERS_SET MEMBER_SET(user) MEMBER_SET(object) MEMBER_SET(callback)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET)
