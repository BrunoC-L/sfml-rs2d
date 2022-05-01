#pragma once
#include "event.h"
#include "units.h"

struct MouseMiddleClickEventData {
    VPixel pos;
};
using MouseMiddleClickEvent = Event<MouseMiddleClickEventData>;
