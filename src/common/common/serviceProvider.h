#pragma once
#include "abstractService.h"
#include <string>
#include <unordered_map>

class ServiceProvider {
public:
	std::unordered_map<std::string, AbstractService*> services;

	void set(std::string name, AbstractService* service) {
		this->services[name] = service;
	}

	AbstractService* get(std::string name) {
		return this->services[name];
	}
};
