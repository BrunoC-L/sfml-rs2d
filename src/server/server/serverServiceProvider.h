#pragma once
#include "serviceProvider.h"
#include "service.h"

class ServerServiceProvider : public ServiceProvider {
	AbstractService* services[SERVICES::SERVICES_SIZE];
public:
	virtual void set(int serviceIndex, AbstractService* service) override {
		services[serviceIndex] = service;
	}

	virtual AbstractService* get(int serviceIndex) override {
		return this->services[serviceIndex];
	}
};
