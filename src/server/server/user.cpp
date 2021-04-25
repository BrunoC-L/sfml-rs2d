#include "user.h"

User::User(): id(-1), isLoggedIn(false) {
	std::cout << "Temp user\n";
}

User::~User() {
	std::cout << "User " << ign << " Disconnected\n";
}

void User::activate(int id, std::string ign, VTile position) {
	this->id = id;
	this->ign = ign;
	this->position = position;
	this->isLoggedIn = true;
	std::cout << "User " << ign << " Connected\n";
}
