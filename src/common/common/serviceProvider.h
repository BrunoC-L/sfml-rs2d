#pragma once
#include <string>
#include <unordered_map>

class Service;

class ServiceProvider {
public:
	virtual void set(int serviceIndex, Service* service) = 0;
	virtual Service* get(int serviceIndex) = 0;
};
