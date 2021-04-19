#pragma once
#include "units.h"
#include "teleportEvent.h"
#include "interactionClickEvent.h"
#include "service.h"

using namespace std;

class Player : public AbstractPlayer, public Service {
private:
	int id = 0;
	VTile position;
    void walk(VTile pos);
public:
    Player(AbstractServiceProvider* provider);
    void init();
	virtual void setID(int id) ;
	virtual const int& getID();
	virtual void setPosition(VTile position);
	virtual VTile& getPosition();
};
