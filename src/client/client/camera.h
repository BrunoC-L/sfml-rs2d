#pragma once
#include "units.h"
#include "abstractServices.h"
#include "service.h"

class Camera : public AbstractCamera, public Service {
	const VTile* position;
public:
	Camera(AbstractServiceProvider* provider);
	void init();
	virtual void setPosition(const VTile* pos);
	virtual const VTile& getPosition();
};
