#pragma once
#include "object.h"

class Resource : public Object {
protected:
	virtual void collect(const std::shared_ptr<User>& user) = 0;
	virtual bool requirementsMet(const std::shared_ptr<User>& user) = 0;
	virtual void giveExperience(const std::shared_ptr<User>& user) = 0;
};
