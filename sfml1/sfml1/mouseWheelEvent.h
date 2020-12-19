#pragma once
#include "eventClassMacro.h"

#include "eventEmitter.cpp"
#include "eventObserver.h"
#include "units.h"
#include "mouseEvent.h"
using namespace std;

#define TYPE MouseWheelEvent
#define PARENTS : public MouseEvent
#define MEMBERS int delta;
#define PARAMETERS VPixel pos, int delta

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS)
