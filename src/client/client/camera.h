#pragma once
#include "units.h"
#include "abstractServices.h"

class Camera : public AbstractCamera {
public:
	Camera(AbstractServiceProvider* provider);
	void init();
	virtual void setPosition(VTile* pos);
	virtual VTile& getPosition();
};
