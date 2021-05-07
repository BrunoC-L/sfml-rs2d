#pragma once
#include "units.h"
#include <string>

class User {
public:
	User();
	~User();
	void activate(int id, std::string ign);

	bool isLoggedIn;
	int id;
	std::string ign;
};
