#include "object.h"
#include "tile.h"
#include "objectStateChangedEvent.h"

Object::Object(std::string fileName, Tile* tile) : fileName(fileName), tile(tile), id(-1), state(0) {
	EVENT(ObjectCreatedEvent, this).emit();
}

const JSON& Object::asJSON() {
	if (!upToDate) {
		upToDate = true;
		repr = JSON();
		repr["fileName"] = fileName;
		repr["name"] = getName();
		repr["state"] = std::to_string(getState());
		repr["x"] = std::to_string(tile->position.x);
		repr["y"] = std::to_string(tile->position.y);
		repr["size"] = "[]";
		repr["size"].push(std::to_string(int(size().x)));
		repr["size"].push(std::to_string(int(size().y)));
		repr["interactions"] = "[]";
		for (const auto& i : getInteractions())
			repr["interactions"].push(i);
	}
	return repr;
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
	upToDate = false;
	EVENT(ObjectStateChangedEvent, this).emit();
}
