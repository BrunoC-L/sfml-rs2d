#pragma once
#include "event.h"
#include "user.h"
#include <memory>
#include <functional>

#undef  TYPE
#undef  PARENTS
#undef  MEMBERS
#undef  PARAMETERS
#undef  MEMBERS_SET
#undef  MEMBERS_INITIALIZER_LIST
#define TYPE SubscribeToInteractionInterruptionEvent
#define PARENTS : public Event
#define MEMBERS const std::shared_ptr<User>& user; std::function<void()> callback;
#define PARAMETERS const std::shared_ptr<User>& user, std::function<void()> callback
#define MEMBERS_SET
#define MEMBERS_INITIALIZER_LIST : MEMBER_INITIALIZER(user), MEMBER_INITIALIZER(callback)

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET, MEMBERS_INITIALIZER_LIST)
