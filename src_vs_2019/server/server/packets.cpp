#include "packets.h"

std::string sanitizeString(std::string arg) {
	if (arg.length() == 0)
		throw std::runtime_error("blocked potential injection");
	for (auto& c : arg) {
		if (isalnum(c) || (c == ' ') || (c == '-') || (c == '_'))
			continue;
		throw std::runtime_error("blocked potential injection");
	}
	return arg;
}

WalkPacket::WalkPacket(const JSON& data) {
	x = data.get("x").asInt();
	y = data.get("y").asInt();
}

LoginPacket::LoginPacket(const JSON& data) {
	username = sanitizeString(data.get("username").asString());
	passwordHashWithBothSalts = sanitizeString(data.get("passwordHash").asString());
}

SignUpPacket::SignUpPacket(const JSON& data) {
	username = sanitizeString(data.get("username").asString());
	passwordHash = sanitizeString(data.get("passwordHash").asString());
}

SaltsRequestPacket::SaltsRequestPacket(const JSON& data) {
	username = sanitizeString(data.get("username").asString());
}
