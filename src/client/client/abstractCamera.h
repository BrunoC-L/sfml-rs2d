#pragma once
#include "units.h"
#include "abstractService.h"

class AbstractCamera : public AbstractService {
public:
	virtual void setPosition(const VTile* pos) = 0;
	virtual const VTile& getPosition() = 0;
};
