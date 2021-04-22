#include "userService.h"

UserService::UserService(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("User", this);
}

void UserService::init() {
	acquire();

    auto onLogin = [&](std::shared_ptr<User> user, JSON json) {
        auto packet = LoginPacket(json);
        dbService->queryPlayerByUsernameEquals(
            packet.username,
            [&, user, packet](QueryResult qr) {
                if (qr.size() == 0)
                    throw std::exception("User does not exist");
                auto userData = qr[0];
                int id = userData["id"].asInt();
                std::string ign = userData["username"].asString();
                int posx = userData["posx"].asInt();
                int posy = userData["posy"].asInt();
                *user = User(id, packet.username, VTile(posx, posy));
                JSON data = "'" + std::to_string(id) + "'";
                server->send(user, "login", data);
                users.push_back(user);
            }
        );
    };
    server->on("login", onLogin, false);
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
            int id = userData["id"].asInt();
            std::string ign = userData["username"].asString();
            int posx = userData["posx"].asInt();
            int posy = userData["posy"].asInt();
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
