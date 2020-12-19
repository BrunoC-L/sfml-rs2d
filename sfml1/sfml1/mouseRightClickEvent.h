#pragma once
#include "eventClassMacro.h"

#include "eventEmitter.cpp"
#include "eventObserver.h"
#include "units.h"
#include "mouseEvent.h"
using namespace std;

#define TYPE MouseRightClickEvent
#define PARENTS : public MouseEvent
#define MEMBERS 
#define PARAMETERS VPixel pos

EVENT_CLASS(TYPE, PARENTS, MEMBERS, PARAMETERS)
