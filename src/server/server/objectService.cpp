#include "objectService.h"

ObjectService::ObjectService(ServiceProvider* provider) : Service(provider) {
	provider->set(OBJECTS, this);
	positionChangeObserver.set([&](PlayerChunkChangeEvent& ev) {
		statePlayerChunk(ev);
	});

	tickObserver.set([&](TickEvent& ev) {
		for (const auto& user : userService->getAllUsers())
			updatePlayerChunk(user);
		for (int x = 0; x < 29; ++x)
			for (int y = 0; y < 25; ++y)
				changes[x][y] = {};
	});

	objectCreatedObserver.set([&](ObjectCreatedEvent& ev) {
		VTile position = ev.object->getTile()->position;
		objects[int(position.x / TILES_PER_CHUNK)][int(position.y / TILES_PER_CHUNK)].push_back(ev.object);
	});

	objectChangedObserver.set([&](ObjectStateChangedEvent& ev) {
		VTile position = ev.object->getTile()->position;
		changes[int(position.x / TILES_PER_CHUNK)][int(position.y / TILES_PER_CHUNK)].push_back(ev.object);
	});
}

void ObjectService::init() {
	acquire();
	server->on("interact", [&](std::shared_ptr<User> user, JSON& json) {
		VTile tile(json["x"].asInt(), json["y"].asInt());
		int objectState = json["object"]["state"].asInt();
		std::string interaction = json["object"]["interaction"].asString();
		std::string objectName = json["object"]["objectName"].asString();
		interact(user, tile, objectState, interaction, objectName);
	}, true);
}

void ObjectService::interact(const std::shared_ptr<User>& user, VTile v, int objectState, const std::string& interaction, std::string objectName) {
	Tile* tile = map->getTile(v);
	if (!tile)
		return;
	const std::shared_ptr<Object>* object = tile->getObject(objectName);
	if (!object || !*object)
		return;
	(*object)->interact(user, objectState, interaction);
}

void ObjectService::updatePlayerChunk(const std::shared_ptr<User>& user) {
	const auto pos = playerActionService->getPlayerPosition(user);
	VChunk centerChunk(int(pos.x / TILES_PER_CHUNK), int(pos.y) / TILES_PER_CHUNK);

	for (int dx = -CHUNK_RADIUS; dx <= CHUNK_RADIUS; ++dx)
		for (int dy = -CHUNK_RADIUS; dy <= CHUNK_RADIUS; ++dy) {
			int dcx = centerChunk.x + dx, dcy = centerChunk.y + dy;
			if (dcx > 0 && dcx < 29 && dcy > 0 && dcy < 25)
				sendUpdates(user, VChunk(dcx, dcy));
		}
}

void ObjectService::statePlayerChunk(PlayerChunkChangeEvent& ev) {
	for (int dx = -CHUNK_RADIUS; dx <= CHUNK_RADIUS; ++dx)
		for (int dy = -CHUNK_RADIUS; dy <= CHUNK_RADIUS; ++dy) {
			int dcx = ev.newChunk.x + dx, dcy = ev.newChunk.y + dy;
			if (dcx > 0 && dcx < 29 && dcy > 0 && dcy < 25)
				sendChunkState(ev.user, VChunk(dcx, dcy));
		}
}

void ObjectService::sendUpdates(const std::shared_ptr<User>& user, VChunk chunk) {
	JSON msg;
	msg["type"] = "objectUpdates";
	msg["data"] = JSON();
	msg["data"]["cx"] = std::to_string(chunk.x);
	msg["data"]["cy"] = std::to_string(chunk.y);
	msg["data"]["cz"] = std::to_string(chunk.z);
	msg["data"]["objects"] = "[]";
	for (Object* obj : changes[int(chunk.x)][int(chunk.y)])
		msg["data"]["objects"].push(obj->asJSON());
	server->send(user, msg);
}

void ObjectService::sendChunkState(const std::shared_ptr<User>& user, VChunk chunk) {
	JSON msg;
	msg["type"] = "objectStates";
	msg["data"] = JSON();
	msg["data"]["cx"] = std::to_string(chunk.x);
	msg["data"]["cy"] = std::to_string(chunk.y);
	msg["data"]["cz"] = std::to_string(chunk.z);
	msg["data"]["objects"] = "[]";
	for (Object* obj : objects[int(chunk.x)][int(chunk.y)])
		msg["data"]["objects"].push(obj->asJSON());
	server->send(user, msg);
}
