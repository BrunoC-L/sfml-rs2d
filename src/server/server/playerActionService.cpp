#include "playerActionService.h"
#include "pathfinder.h"
#include <memory>

PlayerActionService::PlayerActionService(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("PlayerAction", this);
}

void PlayerActionService::init() {
    acquire();

    auto onWalk = [&](std::shared_ptr<User> user, JSON data) {
        auto p1 = user->position;
        int x2 = data["x"].asInt();
        int y2 = data["y"].asInt();
        paths[user] = Pathfinder::pathfind(VTile(p1.x, p1.y), { VTile(x2, y2) }, false, map);
    };

    server->on("walk", onWalk, true);
}

void PlayerActionService::onGameTick() {
    sendPlayerPositions();
    sendGameTick();
}

void PlayerActionService::sendPlayerPositions() {
    JSON msg;
    msg["positions"] = "[]";

    for (auto user : userService->users) {
        if (paths[user].size()) {
            user->position = paths[user][0];
            paths[user].erase(paths[user].begin());
            userService->saveUserPosition(*user);
        }
        JSON pos;
        pos["x"] = std::to_string(user->position.x);
        pos["y"] = std::to_string(user->position.y);
        pos["id"] = std::to_string(user->id);
        msg["positions"].push(pos);
    }

    JSON gameTick;
    gameTick["type"] = "'GameTick'";
    gameTick["data"] = msg;

    for (auto user : userService->users)
        server->send(user, gameTick);
}

void PlayerActionService::sendGameTick() {
    JSON msg;
    msg["type"] = "'tick'";
    for (auto user : userService->users)
        server->send(user, msg);
}
