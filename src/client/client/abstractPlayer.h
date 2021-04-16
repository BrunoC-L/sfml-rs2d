#pragma once
#include <functional>
#include "../../common/common/abstractService.h"

class AbstractPlayer : public AbstractService {
public:
	virtual void setID(int id) = 0;
	virtual const int& getID() = 0;
	virtual void setPosition(VTile position) = 0;
	virtual VTile& getPosition() = 0;
};
