#pragma once
#include "serviceProvider.h"

class Service;

class ServerServiceProvider : public ServiceProvider {
	Service* services[SERVICES::SERVICES_SIZE];
public:
	virtual void set(int serviceIndex, Service* service) override {
		services[serviceIndex] = service;
	}

	virtual Service* get(int serviceIndex) override {
		return this->services[serviceIndex];
	}
};
