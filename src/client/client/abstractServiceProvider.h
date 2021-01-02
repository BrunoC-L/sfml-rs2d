#pragma once
#include "service.h"
#include <string>
#include <unordered_map>
using namespace std;

class AbstractServiceProvider {
public:
	unordered_map<string, Service*> services;

	void set(string name, Service* service) {
		this->services[name] = service;
	}

	Service* get(string name) {
		return this->services[name];
	}

	virtual void init() = 0;
};
