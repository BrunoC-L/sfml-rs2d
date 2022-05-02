#pragma once
#include "event.h"
#include "user.h"
#include <memory>

struct PlayerChunkChangeEventData {
	std::shared_ptr<User> user;
	VChunk newChunk;
	VChunk oldChunk;
	VChunk delta;
};
using PlayerChunkChangeEvent = Event<PlayerChunkChangeEventData>;
