#pragma once
#include "resource.h"

class Rock : public Resource {
protected:
	virtual void prospect(const std::shared_ptr<User>& user);
	virtual void tick(int i, const std::shared_ptr<User>& user) override;
	std::string prospects[2];
public:
	Rock(std::string&& fileName, JSON&& json, Tile* tile) : Resource(std::move(fileName), std::move(json), tile) {}
	virtual void build() override;
	virtual void interact(const std::shared_ptr<User>& user, int objectState, const std::string& interaction) override;
	virtual void setState(int state) override;
};
