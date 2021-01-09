#include "user.h"

User::User(int id) : id(id), isLoggedIn(false) {

}

User::User(int id, std::string ign, VTile position) : id(id), ign(ign), position(position), isLoggedIn(true) {

}
