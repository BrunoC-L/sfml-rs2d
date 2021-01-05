#pragma once
#include "../../common/units.h"
#include "../../common/abstractService.h"

class AbstractCamera : public AbstractService {
public:
	VTile* position;
	virtual void setPosition(VTile* pos) = 0;
	virtual VTile& getPosition() = 0;
};
