#pragma once
#include "event.h"
#include "units.h"

struct MouseMoveEventData {
    VPixel pos;
};
using MouseMoveEvent = Event<MouseMoveEventData>;
