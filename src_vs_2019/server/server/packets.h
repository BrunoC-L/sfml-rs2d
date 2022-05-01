#pragma once
#include "json.h"

std::string sanitizeString(std::string arg);

class WalkPacket {
public:
	int x;
	int y;
	WalkPacket(const JSON& data);
};

class LoginPacket {
public:
	std::string username;
	std::string passwordHashWithBothSalts;
	LoginPacket(const JSON& data);
};

class SignUpPacket {
public:
	std::string username;
	std::string passwordHash;
	SignUpPacket(const JSON& data);
};

class SaltsRequestPacket {
public:
	std::string username;
	SaltsRequestPacket(const JSON& data);
};
