#include "objectService.h"

ObjectService::ObjectService(ServiceProvider* provider) : Service(provider) {
	provider->set(OBJECTS, this);
	positionChangeObserver.set([&](PlayerChunkChangeEvent& ev) {
		updatePlayerChunk(ev);
	});

	tickObserver.set([&](TickEvent& ev) {
		sendUpdates();
	});

	objectCreatedObserver.set([&](ObjectCreatedEvent& ev) {
		VTile position = ev.object->getTile()->position;
		objects[int(position.x / TilesPerChunk)][int(position.y / TilesPerChunk)].push_back(ev.object);
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

void ObjectService::updatePlayerChunk(PlayerChunkChangeEvent& ev) {
	auto diameter = 2 * CHUNK_RADIUS + 1;

	for (int dx = -CHUNK_RADIUS; dx <= CHUNK_RADIUS; ++dx)
		for (int dy = -CHUNK_RADIUS; dy <= CHUNK_RADIUS; ++dy) {
			int dcx = ev.newChunk.x + dx, dcy = ev.newChunk.y + dy;
			if (dcx > 0 && dcx < 29 && dcy > 0 && dcy < 25)
				sendChunkState(ev.user, VChunk(dcx, dcy));
		}
}

void ObjectService::sendUpdates() {
	//JSON msg;
	//msg["type"] = "objectUpdates";
	//msg["data"] = "[]";
	//for (const auto& user : userService->getAllUsers())
	//	server->send(user, msg);
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
