#pragma once
#include "abstractService.h"
#include <string>
#include <unordered_map>

class ServiceProvider {
public:
	virtual void set(int serviceIndex, AbstractService* service) = 0;
	virtual AbstractService* get(int serviceIndex) = 0;
};
