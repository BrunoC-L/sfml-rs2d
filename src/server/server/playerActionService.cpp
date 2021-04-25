#include "playerActionService.h"
#include "pathfinder.h"
#include <memory>
#include "costLogger.h"
#include "loginout.h"
#include "tick.h"

PlayerActionService::PlayerActionService(ServiceProvider* provider) : Service(provider) {
	provider->set("PlayerAction", this);
}

void PlayerActionService::init() {
    acquire();

    auto onWalk = [&](std::shared_ptr<User> user, JSON& data) {
        auto p1 = user->position;
        auto packet = WalkPacket(data);
        paths[user] = Pathfinder::pathfind(VTile(p1.x, p1.y), { VTile(packet.x, packet.y) }, false, map);
    };

    server->on("walk", onWalk, true);

    LogoutEvent::subscribe(
        new EventObserver<LogoutEvent>(
            [&](LogoutEvent* ev) {
                paths.erase(ev->user);
            }
        )
    );

    TickEvent::subscribe(
        new EventObserver<TickEvent>(
            [&](TickEvent* ev) {
                onGameTick();
            }
        )
    );
}

void PlayerActionService::onGameTick() {
    //CostLogger cl("onGameTick.txt");
    sendPlayerPositions();
    sendGameTick();
}

void PlayerActionService::sendPlayerPositions() {
    JSON positions;
    positions["type"] = "positions";
    positions["data"] = "[]";

    for (auto& user : userService->users) {
        if (paths[user].size()) {
            user->position = paths[user][0];
            paths[user].erase(paths[user].begin());
            userService->saveUserPosition(*user);
        }
        JSON pos;
        pos["x"] = std::to_string(user->position.x);
        pos["y"] = std::to_string(user->position.y);
        pos["id"] = std::to_string(user->id);
        positions["data"].push(pos);
    }

    for (auto user : userService->users)
        server->send(user, positions);
}

void PlayerActionService::sendGameTick() {
    JSON msg;
    msg["type"] = "'tick'";
    for (auto user : userService->users)
        server->send(user, msg);
}
