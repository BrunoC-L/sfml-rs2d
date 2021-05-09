#pragma once
#include "object.h"
#include <memory>
#include <string>
#include "json.h"

class ObjectFactory {
public:
	virtual std::shared_ptr<Object> create(JSON& object);
};
