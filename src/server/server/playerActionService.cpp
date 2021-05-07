#include "playerActionService.h"
#include "pathfinder.h"
#include "costLogger.h"
#include "session.h"

PlayerActionService::PlayerActionService(ServiceProvider* provider) : Service(provider) {
	provider->set("PlayerAction", this);
}

void PlayerActionService::init() {
    acquire();

    auto onWalk = [&](std::shared_ptr<User> user, JSON& data) {
        auto packet = WalkPacket(data);
        auto& pp = pathPositions[user];
        auto p1 = pp.position;
        pp.path = Pathfinder::pathfind(VTile(p1.x, p1.y), { VTile(packet.x, packet.y) }, PathFindOption::Onto, map);
    };

    server->on("walk", onWalk, true);

    logoutObserver.set([&](LogoutEvent& ev) {
        pathPositions.erase(ev.user);
        }
    );

    loginObserver.set([&](LoginEvent& ev) {
        pathPositions[ev.user] = { {}, ev.position };
        }
    );

    tickObserver.set([&](TickEvent& ev) {
            onGameTick();
        }
    );
}

void PlayerActionService::updatePlayerPositions() {
    for (int cx = 0; cx < 29; ++cx)
        for (int cy = 0; cy < 25; ++cy)
            positions[cx][cy].clear();

    for (auto it = pathPositions.begin(); it != pathPositions.end(); it++) {
        const auto& user = it->first;
        auto& userPosition = it->second.position;
        if (it->second.path.size()) {
            userPosition = it->second.path[0];
            it->second.path.erase(it->second.path.begin());
            userService->saveUserPosition(*user, userPosition);
        }
        positions[int(userPosition.x / 64)][int(userPosition.y / 64)].push_back({user, userPosition});
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
            if (positions[cx][cy].size() == 0)
                continue;
            for (const auto& userPos : positions[cx][cy]) {
                JSON pos;
                pos["x"] = std::to_string(userPos.second.x);
                pos["y"] = std::to_string(userPos.second.y);
                pos["id"] = std::to_string(userPos.first->id);
                chunks[cx][cy].push_back(pos);
            }
        }

    int radius = 2;

    for (int cx = 0; cx < 29; ++cx)
        for (int cy = 0; cy < 25; ++cy) {
            if (positions[cx][cy].size() == 0)
                continue;
            JSON packet;
            packet["type"] = "positions";
            packet["data"] = "[]";
            for (int dx = -radius; dx <= radius; ++dx)
                for (int dy = -radius; dy <= radius; ++dy) {
                    int dcx = cx + dx, dcy = cy + dy;
                    if (dcx > 0 && dcx < 29 && dcy > 0 && dcy < 25)
                        for (const auto& json : chunks[dcx][dcy])
                            packet["data"].push(json);
                }
            for (const auto& userPos : positions[cx][cy])
                server->send(userPos.first, packet);
        }
}

void PlayerActionService::sendGameTick() {
    JSON msg;
    msg["type"] = "tick";
    for (auto user : userService->users)
        server->send(user, msg);
}

void PlayerActionService::stop() {
}
