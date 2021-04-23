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
	std::string passwordHashWithBothSalts;
	LoginPacket(JSON& data);
};

class SignUpPacket {
public:
	std::string username;
	std::string passwordHash;
	SignUpPacket(JSON& data);
};

class SaltsRequestPacket {
public:
	std::string username;
	SaltsRequestPacket(JSON& data);
};
