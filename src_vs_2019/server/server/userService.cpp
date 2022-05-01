#include "userService.h"
#include "sha256.h"
#include "loginEvent.h"
#include "logger.h"

UserService::UserService(ServiceProvider* provider) : Service(provider) {
	provider->set(USER, this);
    gameMessageObserver.set([&](const GameMessageEvent::Data& ev) {
        JSON data;
        data["message"] = ev.message;
        server->send(ev.user, "chat", data);
    });
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

    availableIndices.reserve(MAX_PLAYERS_ONLINE);
    for (int i = MAX_PLAYERS_ONLINE - 1; i >= 0; --i) {
        availableIndices.push_back(i);
    }

    auto onLogin = [&](std::shared_ptr<User> user, const JSON& json) {
        auto packet = LoginPacket(json);
        auto tempsalt = tempSaltByUser[user];

        // replace with query logindata by username equals, join tables on id

        dbService->queryPlayerByUsernameEquals(
            packet.username,
            [&, user, packet, tempsalt](SelectQueryResult qr) {
                if (qr.size() == 0)
                    return; // TODO alert client that his credentials are bad
                auto userData = qr[0];
                int id = userData["id"].asInt();

                dbService->queryLoginDataByUserId(id, [&, user, userData, packet, tempsalt](SelectQueryResult qr) mutable {
                    if (qr.size() == 0)
                        throw std::runtime_error("Found user matching username but missing LoginData entry in DB");
                    auto userHash = packet.passwordHashWithBothSalts;
                    auto expectedHash = picosha2::hash256_hex_string(tempsalt + qr[0]["hash"].asString());
                    if (userHash != expectedHash)
                        return; // TODO alert client that his credentials are bad

                    std::string ign = userData["username"].asString();
                    int posx = userData["posx"].asInt();
                    int posy = userData["posy"].asInt();
                    JSON data;
                    data["position"] = JSON();
                    data["position"]["x"] = std::to_string(posx);
                    data["position"]["y"] = std::to_string(posy);
                    {
                        std::lock_guard<std::mutex> lg(usersMutex);
                        for (const auto& user : iteratableUsers)
                            if (user->ign == ign)
                                return; // TODO alert client that his account is already logged in
                        auto index = availableIndices.back();
                        availableIndices.erase(availableIndices.end() - 1);
                        user->activate(index, packet.username);
                        data["id"] = std::to_string(index);
                        iteratableUsers.push_back(user);
                    }
                    server->send(user, "login", data);
                    EVENT(LoginEvent, user, VTile(posx, posy)).emit();
                });
            }
        );
    };
    server->on("login", onLogin, false);

    auto onSignUp = [&](std::shared_ptr<User> user, const JSON& json) {
        auto packet = SignUpPacket(json);
        auto permSalt = randomString64();
        auto pwHashWithPermSalt = picosha2::hash256_hex_string(permSalt + packet.passwordHash);
        dbService->selectIdFromPlayerWhereUsernameEquals(packet.username, [&, packet, permSalt, pwHashWithPermSalt](SelectQueryResult qr1) {
            if (qr1.size() != 0)
                return; // account already exists!
            dbService->createPlayerWithUsername(packet.username);
            dbService->selectIdFromPlayerWhereUsernameEquals(packet.username, [&, permSalt, pwHashWithPermSalt](SelectQueryResult qr2) {
                if (qr2.size() == 0)
                    throw std::runtime_error("Just created user but missing in DB, username = " + packet.username);
                auto id = qr2[0]["id"].asString();
                dbService->insertLoginDataIdPermSaltpwHashPermSalt(id, permSalt, pwHashWithPermSalt);
            });
        });
    };
    server->on("sign up", onSignUp, false);

    auto onSaltsRequest = [&](std::shared_ptr<User> user, const JSON& json) {
        auto packet = SaltsRequestPacket(json);
        // select * from logindata where id = (select id from player where username = 'j')
        dbService->selectLogindataWithUsername(packet.username, [&, user](SelectQueryResult qr) {
            if (qr.size() == 0)
                return; // no user matches, TODO alert client
            auto permSalt = qr[0]["salt"].asString();
            auto tempSalt = randomString64();
            JSON data;
            data["permsalt"] = permSalt;
            data["tempsalt"] = tempSalt;
            tempSaltByUser[user] = tempSalt;
            scheduler->callInTicks(5, [&, user]() {
                tempSaltByUser.erase(user);
            });
            server->send(user, "salts", data);
        });
    };

    server->on("salts request", onSaltsRequest, false);
    logoutObserver.set([&](const LogoutEvent::Data& ev) {
        logout(ev.user);
    });

    server->on("chat", [&](std::shared_ptr<User> user, const JSON& json) {
        JSON data;
        data["message"] = json.get("message");
        data["sender"] = user->ign;
        for (const auto& user : getAllUsers())
            server->send(user, "chat", data);
    }, true);
}

#ifdef __APPLE__
#define _ASSERT assert
#endif // __APPLE__

void UserService::logout(const std::shared_ptr<User>& user) {
    std::lock_guard<std::mutex> lg(usersMutex);
    _ASSERT(user->isLoggedIn);
    users[user->index].reset();
    iteratableUsers.erase(std::find(iteratableUsers.begin(), iteratableUsers.end(), user));
    Logging::Server::log_default(std::to_string(iteratableUsers.size()) + " users online");
    availableIndices.push_back(user->index);
    Logging::Server::log_default("unlocking");
}

const std::shared_ptr<User>& UserService::getUserByIndex(int index) {
    return users[index];
}

const std::vector<std::shared_ptr<User>>& UserService::getAllUsers() {
    return iteratableUsers;
}