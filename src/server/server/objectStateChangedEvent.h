#pragma once
#include "../../common/common/event.h"

class Object;

struct ObjectStateChangedEventData {
	Object* object;
};
using ObjectStateChangedEvent = Event<ObjectStateChangedEventData>;
