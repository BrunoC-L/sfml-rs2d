#pragma once
#include "object.h"
#include "tickEvent.h"
#include "goToObjectRequestEvent.h"
#include "subscribeToInteractionInterruption.h"
#include "tile.h"
#include "movingPredicate.h"
#include "random.h"
#include "scheduleTaskEvent.h"

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
	std::vector<std::pair<int, std::shared_ptr<User>>> interactors;
	EventObserver<TickEvent> tickObserver;
	void tick();
	virtual void tick(int i, const std::shared_ptr<User>& user) = 0;
	JSON json;
public:
	Resource(std::string&& fileName, JSON&& json, Tile* tile);
	virtual void build() override;
	virtual const std::vector<VTile>& getInteractibleTiles() override;
	virtual VTile size() override;
	virtual const std::vector<std::string>& getInteractions() override;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, const std::string& interaction) = 0;
	virtual const std::string& getName() override;
	virtual void setState(int state) override;
};
