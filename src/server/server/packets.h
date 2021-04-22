#pragma once
#include "json.h"

std::string sanitizeString(std::string arg);

class WalkPacket {
public:
	int x;
	int y;
	WalkPacket(JSON& data);
};

class LoginPacket {
public:
	std::string username;
	std::string passwordHash;
	LoginPacket(JSON& data);
};
