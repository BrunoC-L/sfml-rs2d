#include "object.h"
#include "tile.h"
#include "objectStateChangedEvent.h"

const JSON& Object::asJSON() {
	if (!upToDate) {
		json = JSON();
		json["name"] = getName();
		json["state"] = std::to_string(getState());
		json["x"] = std::to_string(tile->position.x);
		json["y"] = std::to_string(tile->position.y);
		json["interactions"] = "[]";
		for (const auto& i : getInteractions())
			json["interactions"].push(i);
	}
	return json;
}

Object::Object(Tile* tile) : tile(tile), id(-1), state(0) {
	ObjectCreatedEvent(this).emit();
}

unsigned Object::getId() {
	return id;
}

void Object::setId(unsigned id) {
	this->id = id;
}

const Tile* Object::getTile() {
	return tile;
}

int Object::getState() {
	return state;
}

void Object::setState(int state) {
	this->state = state;
	ObjectStateChangedEvent(this).emit();
}
