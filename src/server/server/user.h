#pragma once
#include "../../common/units.h"
#include <string>

class User {
public:
	User(int id);
	User(int id, std::string ign, VTile position);
	int id;
	std::string ign;
	VTile position;
};