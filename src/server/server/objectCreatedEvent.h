#pragma once
#include "../../common/common/event.h"

class Object;

struct ObjectCreatedEventData {
	Object* object;
};
using ObjectCreatedEvent = Event<ObjectCreatedEventData>;