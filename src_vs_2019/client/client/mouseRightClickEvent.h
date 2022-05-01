#pragma once
#include "event.h"
#include "units.h"

struct MouseRightClickEventData {
    VPixel pos;
};
using MouseRightClickEvent = Event<MouseRightClickEventData>;
