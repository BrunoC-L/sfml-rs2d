#pragma once
#include "abstractServiceMacro.h"
#include "units.h"

SERVICES;

class AbstractCamera : public Service {
	SERVICE_MEMBERS;
	VTile* position;
	virtual void setPosition(VTile* pos) = 0;
	virtual VTile& getPosition() = 0;
};
