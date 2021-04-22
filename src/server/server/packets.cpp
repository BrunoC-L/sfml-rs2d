#include "packets.h"

std::string sanitizeString(std::string arg) {
	if (arg.length() == 0)
		throw std::exception("blocked potential injection");
	for (auto& c : arg) {
		if (isalnum(c) || (c == ' ') || (c == '-') || (c == '_'))
			continue;
		throw std::exception("blocked potential injection");
	}
	return arg;
}

WalkPacket::WalkPacket(JSON& data) {
	x = data["x"].asInt();
	y = data["y"].asInt();
}

LoginPacket::LoginPacket(JSON& data) {
	username = sanitizeString(data["username"].asString());
	passwordHash = sanitizeString(data["passwordHash"].asString());
}
