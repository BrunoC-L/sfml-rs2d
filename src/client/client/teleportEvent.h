#pragma once
#include "event.h"
#include "units.h"

struct TeleportEventData {
    VTile pos;
};
using TeleportEvent = Event<TeleportEventData>;
