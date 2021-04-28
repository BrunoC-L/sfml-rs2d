#include "userService.h"
#include "sha256.h"

UserService::UserService(ServiceProvider* provider) : Service(provider) {
	provider->set("User", this);
}

std::string randomString64() {
    std::string str;
    srand((unsigned)time(NULL));
    char chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    str.reserve(64);
    for (int i = 0; i < 64; ++i)
        str += chars[(int)((double)rand() / ((double)RAND_MAX + 1) * 16)];
    return str;
}

void UserService::init() {
	acquire();

    auto onLogin = [&](std::shared_ptr<User> user, JSON& json) {
        auto packet = LoginPacket(json);
        auto tempsalt = tempSaltByUser[user];
        dbService->queryPlayerByUsernameEquals(
            packet.username,
            [&, user, packet, tempsalt](SelectQueryResult qr) {
                if (qr.size() == 0)
                    throw std::exception("User does not exist");
                auto userData = qr[0];
                int id = userData["id"].asInt();

                dbService->queryLoginDataByUserId(id, [&, id, user, userData, packet, tempsalt](SelectQueryResult qr) mutable {
                    if (qr.size() == 0)
                        throw std::exception("Found user matching username but missing LoginData entry in DB");
                    auto userHash = packet.passwordHashWithBothSalts;
                    auto expectedHash = picosha2::hash256_hex_string(tempsalt + qr[0]["hash"].asString());
                    if (userHash != expectedHash)
                        return; // silent fail, terrible for the client, will do later

                    std::string ign = userData["username"].asString();
                    int posx = userData["posx"].asInt();
                    int posy = userData["posy"].asInt();
                    user->activate(id, packet.username, VTile(posx, posy));
                    JSON data;
                    data["id"] = "'" + std::to_string(id) + "'";
                    data["position"] = JSON();
                    data["position"]["x"] = std::to_string(posx);
                    data["position"]["y"] = std::to_string(posy);
                    server->send(user, "login", data);
                    users.push_back(user);
                });
            }
        );
    };
    server->on("login", onLogin, false);

    auto onSignUp = [&](std::shared_ptr<User> user, JSON& json) {
        auto packet = SignUpPacket(json);
        auto permSalt = randomString64();
        auto pwHashWithPermSalt = picosha2::hash256_hex_string(permSalt + packet.passwordHash);
        dbService->selectQuery("select id from player where username = '" + packet.username + "';", [&, packet, permSalt, pwHashWithPermSalt](SelectQueryResult qr) {
            if (qr.size() != 0)
                return; // account already exists!
            dbService->nonSelectQuery("insert into player (username) values ('" + packet.username + "');");
            dbService->selectQuery("select id from player where username = '" + packet.username + "';", [&, permSalt, pwHashWithPermSalt](SelectQueryResult qr) {
                if (qr.size() == 0)
                    throw std::exception("Just created user but missing in DB");
                auto id = qr[0]["id"].asString();
                dbService->nonSelectQuery("insert into logindata values(" + id + ", '" + permSalt + "', '" + pwHashWithPermSalt + "');");
            });
        });
    };
    server->on("sign up", onSignUp, false);

    auto onSaltsRequest = [&](std::shared_ptr<User> user, JSON& json) {
        auto packet = SaltsRequestPacket(json);
        dbService->selectQuery("declare @id int set @id = (select id from player where username = '" + packet.username + "'); select * from logindata where id = @id", [&, user](SelectQueryResult qr) {
            if (qr.size() == 0)
                return; // no user matches
            auto permSalt = qr[0]["salt"].asString();
            auto tempSalt = randomString64();
            JSON json;
            json["type"] = "'salts'";
            json["data"] = JSON();
            json["data"]["permsalt"] = permSalt;
            json["data"]["tempsalt"] = tempSalt;
            tempSaltByUser[user] = tempSalt;
            scheduler->callInTicks(5, [&, user]() {
                tempSaltByUser.erase(user);
            });
            server->send(user, json);
        });
    };

    server->on("salts request", onSaltsRequest, false);
    logoutObserver.set([&](LogoutEvent& ev) {
        logout(*(ev.user));
    });
}

void UserService::saveUserPosition(User& user) {
    std::string q = "update Player set posx = " +
        std::to_string(user.position.x) +
        ", posy = " +
        std::to_string(user.position.y) +
        " where id = '" + std::to_string(user.id) + "'\n";
    dbService->nonSelectQuery(q);
}

void UserService::logout(User& user) {
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

void UserService::stop() {
}
