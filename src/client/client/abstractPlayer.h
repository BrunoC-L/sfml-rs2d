#pragma once
#include <functional>
#include "abstractService.h"

class AbstractPlayer : public AbstractService {
public:
	virtual void setID(int id) = 0;
	virtual const int& getID() = 0;
	virtual void setPosition(VTile position) = 0;
	virtual void setIntPosition(VTile position) = 0;
	virtual const VTile& getPosition() = 0;
	virtual const VTile& getIntPosition() = 0;
};
