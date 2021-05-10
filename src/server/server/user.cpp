#include "user.h"

User::User(): index(-1), isLoggedIn(false) {
}

User::~User() {
	std::cout << "User " << ign << " Disconnected\n";
}

void User::activate(int index, std::string ign) {
	this->index = index;
	this->ign = ign;
	this->isLoggedIn = true;
	std::cout << "User " << ign << " Connected\n";
}
