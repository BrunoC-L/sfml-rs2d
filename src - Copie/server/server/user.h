#pragma once
#include "units.h"
#include <string>

class User {
public:
	User();
	~User();
	void activate(int index, std::string ign);

	bool isLoggedIn;
	int index;
	std::string ign;
};
