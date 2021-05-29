#pragma once
#include "object.h"

struct ObjectState {
	std::string name;
	std::string examine;
	std::vector<std::string> interactions;
	std::vector<VTile> interactibleTiles;
	VTile size;
	bool calculatedInteractibleTiles;
};

class Resource : public Object {
protected:
	virtual void collect(const std::shared_ptr<User>& user);
	virtual void examine(const std::shared_ptr<User>& user);
	virtual bool requirementsMet(const std::shared_ptr<User>& user);
	virtual void giveExperience(const std::shared_ptr<User>& user);
	std::vector<ObjectState> states;
private:
	int UP = 0, DOWN = 1;
	int COLLECT = 0;
public:
	Resource(JSON&& json, Tile* tile);
	virtual const std::vector<VTile>& getInteractibleTiles() override;
	virtual VTile size() override;
	virtual const std::vector<std::string>& getInteractions() override;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, const std::string& interaction) override;
	virtual const std::string& getName() override;
};
