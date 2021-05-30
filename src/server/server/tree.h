#pragma once
#include "resource.h"

class Tree : public Resource {
protected:
	virtual void tick(int i, const std::shared_ptr<User>& user) override;
public:
	Tree(std::string&& fileName, JSON&& json, Tile* tile) : Resource(std::move(fileName), std::move(json), tile) {}
	virtual void build() override;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, const std::string& interaction) override;
};
