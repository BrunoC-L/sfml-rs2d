#include "object.h"
#include "tile.h"

JSON Object::asJSON() {
	JSON json;
	json["name"] = getName();
	json["state"] = std::to_string(getState());
	json["x"] = std::to_string(tile->position.x);
	json["y"] = std::to_string(tile->position.y);
	json["interactions"] = "[]";
	for (const auto& i : getInteractions())
		json["interactions"].push(i);
	return json;
}
