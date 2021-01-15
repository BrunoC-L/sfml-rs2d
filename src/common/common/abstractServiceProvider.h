#pragma once
#include "abstractService.h"
#include <string>
#include <unordered_map>
using namespace std;

class AbstractServiceProvider {
public:
	unordered_map<string, AbstractService*> services;

	void set(string name, AbstractService* service) {
		this->services[name] = service;
	}

	AbstractService* get(string name) {
		return this->services[name];
	}
};
