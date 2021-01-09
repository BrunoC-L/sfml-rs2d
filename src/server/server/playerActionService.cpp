#include "playerActionService.h"
#include "pathfinder.h"
#include <memory>

PlayerActionService::PlayerActionService(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("PlayerAction", this);
}

void PlayerActionService::onGameTick() {
	for (auto action : actions)
		return;

    // for every chunk
    //   store chunk data to send
    // for every player
    //   send data for each chunk he sees
    //   send personal data

    JSON msg;
    msg["type"] = "'positions'";
    msg["data"] = "[]";

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
        msg["data"].push(pos);
    }

    //for (int i = 0; i < positions.size(); ++i) {
    //    JSON pos;
    //    pos["x"] = std::to_string(positions[i].x);
    //    pos["y"] = std::to_string(positions[i].y);
    //    pos["id"] = std::to_string(i);
    //    msg["data"].push(pos);
    //}

    for (auto user : userService->users)
        server->send(user, msg);
}

void PlayerActionService::onAction(std::shared_ptr<PlayerAction> action) {
	actions.push_back(action);
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
