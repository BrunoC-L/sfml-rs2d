#pragma once
#include "units.h"
#include "event.h"

struct MouseWheelEventData {
    VPixel pos;
    int delta;
};
using MouseWheelEvent = Event<MouseWheelEventData>;
