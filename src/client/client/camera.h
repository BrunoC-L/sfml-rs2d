#pragma once
#include "../../common/units.h"
#include "abstractServices.h"
#include "service.h"

class Camera : public AbstractCamera, public Service {
public:
	Camera(AbstractServiceProvider* provider);
	void init();
	virtual void setPosition(VTile* pos);
	virtual VTile& getPosition();
};
