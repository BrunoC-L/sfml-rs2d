#pragma once
#include "abstractTree.h"

class Tree : public AbstractTree {
protected:
	virtual void collect(const std::shared_ptr<User>& user) override;
	virtual bool requirementsMet(const std::shared_ptr<User>& user) override;
	virtual void giveExperience(const std::shared_ptr<User>& user) override;
	virtual void examineUp(const std::shared_ptr<User>& user) override;
	virtual void examineStump(const std::shared_ptr<User>& user) override;
public:
	Tree(JSON&& json, Tile* tile);
	virtual int getState() override;
	virtual std::string getName() override;
	~Tree() {
		throw 1;
	}
};
