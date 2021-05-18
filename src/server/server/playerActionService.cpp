#include "playerActionService.h"
#include "pathfinder.h"
#include "costLogger.h"
#include "session.h"

PlayerActionService::PlayerActionService(ServiceProvider* provider) : Service(provider) {
	provider->set(PLAYERACTION, this);
}

void PlayerActionService::init() {
    acquire();

    auto onWalk = [&](std::shared_ptr<User> user, JSON& data) {
        auto packet = WalkPacket(data);
        walk(user, packet);
    };

    server->on("walk", onWalk, true);

    tickObserver.set([&](TickEvent& ev) {
            onGameTick();
        }
    );

    loginObserver.set([&](LoginEvent& ev) {
        pathPositions[ev.user->index] = { {}, ev.position };
    });

    goToObjectObserver.set([&](GoToObjectRequest& ev) {
        walk(ev.user, ev.object->getInteractibleTiles());
        movementCompleteCallbacks[ev.user->index] = std::make_shared<std::function<void()>>(ev.callback);
    });
}

void PlayerActionService::walk(std::shared_ptr<User> user, WalkPacket& packet) {
    //resourceService->interact(user, VTile(1164, 863), 0, 0, 0);
    walk(user, { VTile(packet.x, packet.y) });
}

void PlayerActionService::walk(std::shared_ptr<User> user, std::vector<VTile> destination) {
    auto& pp = pathPositions[user->index];
    auto p1 = pp.position;
    pp.path = Pathfinder::pathfind(p1, destination, PathFindOption::Onto, map);
}

void PlayerActionService::updatePlayerPositions() {
    for (int cx = 0; cx < 29; ++cx)
        for (int cy = 0; cy < 25; ++cy)
            positions[cx][cy].clear();

    for (const auto& user : userService->getAllUsers()) {
        auto& pathPosition = pathPositions[user->index];
        auto& userPosition = pathPosition.position;
        if (pathPosition.path.size()) {
            userPosition = pathPosition.path[0];
            pathPosition.path.erase(pathPosition.path.begin());
            PlayerMoveEvent(user, userPosition).emit();
        }
        auto& s = movementCompleteCallbacks[user->index];
        if (s) {
            (*s)();
            s = nullptr;
        }
        positions[int(userPosition.x / 64)][int(userPosition.y / 64)].push_back({ user, userPosition });
    }
}

void PlayerActionService::onGameTick() {
    updatePlayerPositions();
    sendPlayerPositions();
    sendGameTick();
}

void PlayerActionService::sendPlayerPositions() {
    auto path = getSession().get("logs").get("server").asString();
    std::vector<JSON> chunks[29][25];
    for (int cx = 0; cx < 29; ++cx)
        for (int cy = 0; cy < 25; ++cy) {
            if (!positions[cx][cy].size())
                continue;
            chunks[cx][cy].reserve(positions[cx][cy].size());
            for (const auto& userPos : positions[cx][cy]) {
                JSON pos;
                pos["x"] = std::to_string(userPos.second.x);
                pos["y"] = std::to_string(userPos.second.y);
                pos["id"] = std::to_string(userPos.first->index);
                chunks[cx][cy].push_back(pos);
            }
        }

    int radius = 2;

    for (int cx = 0; cx < 29; ++cx)
        for (int cy = 0; cy < 25; ++cy) {
            if (!positions[cx][cy].size())
                continue;
            JSON packet;
            packet["type"] = "positions";
            packet["data"] = "[]";
            for (int dx = -radius; dx <= radius; ++dx)
                for (int dy = -radius; dy <= radius; ++dy) {
                    int dcx = cx + dx, dcy = cy + dy;
                    if (dcx > 0 && dcx < 29 && dcy > 0 && dcy < 25)
                        for (auto& json : chunks[dcx][dcy])
                            packet["data"].push(std::move(json));
                }
            for (const auto& userPos : positions[cx][cy])
                server->send(userPos.first, packet);
        }
}

void PlayerActionService::sendGameTick() {
    JSON msg;
    msg["type"] = "tick";
    for (auto user : userService->getAllUsers())
        server->send(user, msg);
}
