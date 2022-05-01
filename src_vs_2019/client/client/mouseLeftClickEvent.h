#pragma once
#include "event.h"
#include "units.h"

struct MouseLeftClickEventData {
    VPixel pos;
};
using MouseLeftClickEvent = Event<MouseLeftClickEventData>;
