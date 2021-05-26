#pragma once
#include "object.h"

class Resource : public Object {
protected:
	virtual void collect(const std::shared_ptr<User>& user);
	virtual bool requirementsMet(const std::shared_ptr<User>& user);
	virtual void giveExperience(const std::shared_ptr<User>& user);
	int state;
	std::vector<std::string> examines;
	std::vector<std::string> names;
	std::vector<std::vector<std::string>> interactions;
	std::vector<VTile> interactibleTiles;
	bool calculatedTiles = false;
public:
	Resource(JSON&& json, Tile* tile);
	virtual const std::vector<VTile>& getInteractibleTiles() override;
	virtual VTile size() override;
	virtual int getState() override;
	virtual const std::vector<std::string>& getInteractions() override;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, int index) override;
	virtual const std::string& getName() override;
};
