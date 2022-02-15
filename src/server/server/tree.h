#pragma once
#include "resource.h"

class Tree : public Resource {
protected:
	enum States { UP, DOWN };
	virtual void tick(Interactor i) override;
public:
	Tree(std::string&& fileName, JSON&& json, Tile* tile) : Resource(std::move(fileName), std::move(json), tile) {}
	virtual void build() override;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, const std::string& interaction) override;
};
