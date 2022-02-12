#include "inMemoryDB.h"

InMemoryDB::InMemoryDB(ServiceProvider* provider) : Service(provider) {
	provider->set(DATABASE, this);
}

void InMemoryDB::stop() {

}

void InMemoryDB::init() {

}

void InMemoryDB::queryPlayerByUsernameEquals(std::string username, std::function<void(SelectQueryResult)> f) {

}

void InMemoryDB::queryLoginDataByUserId(int id, std::function<void(SelectQueryResult)> f) {

}

void InMemoryDB::selectIdFromPlayerWhereUsernameEquals(std::string username, std::function<void(SelectQueryResult)> f) {

}

void InMemoryDB::createPlayerWithUsername(std::string username) {

}

void InMemoryDB::insertLoginDataIdPermSaltpwHashPermSalt(std::string id, std::string permSalt, std::string hash) {

}

void InMemoryDB::selectLogindataWithUsername(std::string username, std::function<void(SelectQueryResult)> f) {

}
