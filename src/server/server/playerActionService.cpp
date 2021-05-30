#include "playerActionService.h"
#include "pathfinder.h"
#include "costLogger.h"
#include "session.h"
#include "playerPositionChangeEvent.h"
#include "playerChunkChangeEvent.h"

PlayerActionService::PlayerActionService(ServiceProvider* provider) : Service(provider) {
	provider->set(PLAYERACTION, this);
    chunks = std::vector<std::vector<std::vector<std::shared_ptr<User>>>>(29, std::vector<std::vector<std::shared_ptr<User>>>(25));
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
        oldPositions[ev.user->index] = ev.position;
        movementCompleteCallbacks[ev.user->index] = nullptr;
        interactionInterruptionCallbacks[ev.user->index] = nullptr;
        VChunk chunk(int(ev.position.x / TilesPerChunk), int(ev.position.y / TilesPerChunk));
        chunks[chunk.x][chunk.y].push_back(ev.user);
        PlayerPositionChangeEvent(ev.user, ev.position, ev.position, VTile()).emit();
        PlayerChunkChangeEvent(ev.user, chunk, chunk, VChunk()).emit();
    });

    logoutObserver.set([&](LogoutEvent& ev) {
        VTile position = pathPositions[ev.user->index].position;
        VChunk vchunk(int(position.x / TilesPerChunk), int(position.y / TilesPerChunk));
        auto& chunk = chunks[vchunk.x][vchunk.y];
        chunk.erase(std::find(chunk.begin(), chunk.end(), ev.user));
    });

    goToObjectObserver.set([&](GoToObjectRequest& ev) {
        walk(ev.user, ev.object->getInteractibleTiles());
        movementCompleteCallbacks[ev.user->index] = ev.callback;
    });

    interruptionSubscriptionObserver.set([&](SubscribeToInteractionInterruptionEvent& ev) {
        interactionInterruptionCallbacks[ev.user->index] = ev.callback;
    });
}

const std::vector<std::vector<std::vector<std::shared_ptr<User>>>>& PlayerActionService::getUsersByChunk() {
    return chunks;
}

VTile PlayerActionService::getPlayerPosition(const std::shared_ptr<User>& user) {
    return pathPositions[user->index].position;
}

void PlayerActionService::walk(std::shared_ptr<User> user, WalkPacket& packet) {
    walk(user, { VTile(packet.x, packet.y) });
}

void PlayerActionService::walk(std::shared_ptr<User> user, std::vector<VTile> destination) {
    auto& s = interactionInterruptionCallbacks[user->index];
    if (s) {
        s();
        s = nullptr;
    }
    movementCompleteCallbacks[user->index] = nullptr;
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
        else {
            auto& s = movementCompleteCallbacks[user->index];
            if (s) {
                s();
                s = nullptr;
            }
        }
        positions[int(userPosition.x / TilesPerChunk)][int(userPosition.y / TilesPerChunk)].push_back({ user, userPosition });
    }
}

void PlayerActionService::onGameTick() {
    updatePlayerPositions();
    sendPlayerPositions();
    sendGameTick();
    checkForTileAndChunkChanges();
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

    for (int cx = 0; cx < 29; ++cx)
        for (int cy = 0; cy < 25; ++cy) {
            if (!positions[cx][cy].size())
                continue;
            JSON packet;
            packet["type"] = "positions";
            packet["data"] = "[]";
            for (int dx = -CHUNK_RADIUS; dx <= CHUNK_RADIUS; ++dx)
                for (int dy = -CHUNK_RADIUS; dy <= CHUNK_RADIUS; ++dy) {
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

void PlayerActionService::checkForTileAndChunkChanges() {
    for (int x = 0; x < 29; ++x)
        for (int y = 0; y < 25; ++y) {
            auto& chunk = chunks[x][y];
            for (const auto& user : chunk) {
                VTile position = pathPositions[user->index].position;
                if (position != oldPositions[user->index]) {
                    VChunk newChunk(int(position.x / TilesPerChunk), int(position.y / TilesPerChunk));
                    VChunk oldChunk(x, y);
                    PlayerPositionChangeEvent(user, position, oldPositions[user->index], position - oldPositions[user->index]).emit();
                    oldPositions[user->index] = position;
                    if (newChunk != oldChunk) {
                        chunk.erase(std::find(chunk.begin(), chunk.end(), user));
                        chunks[newChunk.x][newChunk.y].push_back(user);
                        PlayerChunkChangeEvent(user, newChunk, oldChunk, newChunk - oldChunk).emit();
                    }
                }
            }
        }
}
