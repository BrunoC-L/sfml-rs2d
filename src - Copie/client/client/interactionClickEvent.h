#pragma once
#include "event.h"
#include "units.h"
#include <functional>

struct InteractionClickEventData {
    std::function<bool(void)> f;
};
using InteractionClickEvent = Event<InteractionClickEventData>;
