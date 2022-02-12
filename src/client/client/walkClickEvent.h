#pragma once
#include "event.h"
#include "units.h"
#include <functional>

struct WalkClickEventData {
    VTile pos;
};
using WalkClickEvent = Event<WalkClickEventData>;
