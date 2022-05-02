#include "inMemoryDB.h"

InMemoryDB::InMemoryDB(ServiceProvider* provider) : Service(provider) {
	provider->set(DATABASE, this);
}

void InMemoryDB::stop() {
    
}

void InMemoryDB::init() {

}

void InMemoryDB::queryPlayerByUsernameEquals(std::string username, std::function<void(SelectQueryResult)> f) {
    for(const auto& player: playerTable) {
        if(player.get("username").asString() == username) {
            f({player});
            return;
        }
    }
    f({});
}

void InMemoryDB::queryLoginDataByUserId(int id, std::function<void(SelectQueryResult)> f) {
    for(const auto& data: loginDataTable) {
        if(data.get("id").asInt() == id) {
            f({data});
            return;
        }
    }
    f({});
}

void InMemoryDB::selectIdFromPlayerWhereUsernameEquals(std::string username, std::function<void(SelectQueryResult)> f) {
    queryPlayerByUsernameEquals(username, f);
}

void InMemoryDB::createPlayerWithUsername(std::string username) {
    size_t playerId = playerTable.size();
    std::pair<int,int> spawn = {1172, 869};
    playerTable.emplace_back("{'id':" + std::to_string(playerId) + ",'username':'" + username + "', 'posx': " + std::to_string(spawn.first) + ", 'posy': " + std::to_string(spawn.second) + "}");
}

void InMemoryDB::insertLoginDataIdPermSaltpwHashPermSalt(std::string id, std::string permSalt, std::string hash) {
    loginDataTable.emplace_back("{'id':" + id + ",'salt':'" + permSalt + "','hash':'" + hash + "'}");
}

void InMemoryDB::selectLogindataWithUsername(std::string username, std::function<void(SelectQueryResult)> f) {
    for(const auto& player: playerTable) {
        if(player.get("username").asString() == username) {
            int id = player.get("id").asInt();
            queryLoginDataByUserId(id, f);
            return;
        }
    }
    f({});
}
