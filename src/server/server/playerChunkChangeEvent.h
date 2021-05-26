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
#define TYPE PlayerChunkChangeEvent
#define PARENTS : public Event
#define MEMBERS std::shared_ptr<User> user; VChunk newChunk; VChunk oldChunk; VChunk delta;
#define PARAMETERS std::shared_ptr<User> user, VChunk newChunk, VChunk oldChunk, VChunk delta
#define MEMBERS_SET MEMBER_SET(user) MEMBER_SET(newChunk) MEMBER_SET(oldChunk) MEMBER_SET(delta)
#define MEMBERS_INITALIZER_LIST

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS, MEMBERS_SET, MEMBERS_INITALIZER_LIST)
