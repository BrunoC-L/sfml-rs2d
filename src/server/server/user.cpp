#include "user.h"
#include "logger.h"

User::User(): index(-1), isLoggedIn(false) {
}

User::~User() {
	Logging::Server::log_default("User " + ign + " Disconnected\n");
}

void User::activate(int index, std::string ign) {
	this->index = index;
	this->ign = ign;
	this->isLoggedIn = true;
	Logging::Server::log_default("User " + ign + " Connected\n");
}
