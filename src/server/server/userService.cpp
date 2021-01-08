#include "userService.h"

UserService::UserService(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("User", this);
}

void UserService::init() {
	acquire();

    auto onLogin = [&](std::shared_ptr<User> user, JSON json) {
        std::string username = json["username"].asString();
        int id = json["id"].asInt();
        user->ign = username;
        user->id = id;
        dbService->queryPlayerByUsernameEquals(
            username,
            [&, user](QueryResult qr) {
                auto res = qr[0];
                int posx = atoi(res[4].c_str());
                int posy = atoi(res[5].c_str());
                user->position = VTile(posx, posy);
                // emit user authentified
            }
        );
    };
    server->on("login", onLogin);
}

void UserService::saveUserPosition(User user) {
    std::string q = "update Player set posx = " +
        std::to_string(user.position.x) +
        ", posy = " +
        std::to_string(user.position.y) +
        " where id = '" + std::to_string(user.id) + "'\n";
    dbService->query(q);
}

void UserService::login(std::string username, std::function<void(std::shared_ptr<User>)> onSuccess) {
    dbService->queryPlayerByUsernameEquals(
        username,
        [&](QueryResult qr) {
            if (qr.size() == 0)
                throw std::exception("User does not exist");
            auto userData = qr[0];
            int id = atoi(userData[0].c_str());
            std::string ign = userData[2];
            int posx = atoi(userData[4].c_str());
            int posy = atoi(userData[5].c_str());
            users.push_back(std::make_shared<User>(id, ign, VTile(posx, posy)));
            onSuccess(users.back());
        }
    );
}

void UserService::logout(User user) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        std::shared_ptr<User> userIt = *it;
        if (user.id == userIt->id) {
            users.erase(it);
            return;
        }
    }
}

std::shared_ptr<User> UserService::getUserById(int id) {
    for (auto user : users)
        if (user->id == id)
            return user;
    throw std::exception();
}
