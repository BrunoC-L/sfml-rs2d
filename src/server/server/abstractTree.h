#pragma once
#include "resource.h"

class ItemStack;

class AbstractTree : public Resource {
protected:
	virtual void examineUp(const std::shared_ptr<User>& user) = 0;
	virtual void examineStump(const std::shared_ptr<User>& user) = 0;
	int state;
	const int UP = 0, STUMP = 1;
	const int CHOP = 0, EXAMINE = 1;
public:
	virtual VTile size() override;
	virtual std::vector<std::string> getInteractions() override;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, int index) override;
	virtual int getState() override;
};
