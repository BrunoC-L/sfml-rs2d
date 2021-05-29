#include "resource.h"
#include "tile.h"
#include "movingPredicate.h"
#include "goToObjectRequestEvent.h"

Resource::Resource(JSON&& json, Tile* tile) : Object(tile) {
	const auto& objects = json.getChildren();
	states.reserve(objects.size());
	for (const auto& objectState : objects) {
		ObjectState state;
		state.calculatedInteractibleTiles = false;
		state.name = objectState.get("name").asString();
		state.examine = objectState.get("examine").asString();
		state.size = VTile(objectState.get("size").getChildren()[0].asInt(), objectState.get("size").getChildren()[1].asInt());
		for (const auto& i : objectState.get("interactions").getChildren()) {
			state.interactions.push_back(i.get("name").asString());
		}
		states.push_back(state);
	}
}

void Resource::collect(const std::shared_ptr<User>& user) {
	if (state != 0)
		return;
	GoToObjectRequest(user, this, [&]() {
		setState(1);
		std::cout << "Hello World!\n";
	}).emit();
}

void Resource::examine(const std::shared_ptr<User>& user) {
	auto& object = states[state];
	std::cout << object.examine << std::endl;
}

bool Resource::requirementsMet(const std::shared_ptr<User>& user) {
	return true;
}

void Resource::giveExperience(const std::shared_ptr<User>& user) {
}

const std::vector<VTile>& Resource::getInteractibleTiles() {
	auto& objectState = states[state];
	if (!objectState.calculatedInteractibleTiles) {
		objectState.calculatedInteractibleTiles = true;
		std::vector<VTile> occupied = {};
		const auto size = this->size();
		for (int x = 0; x < size.x; ++x)
			for (int y = 0; y < size.y; ++y)
				occupied.push_back(getTile()->position + VTile(x, y));
		objectState.interactibleTiles = MovingPredicate::getNextTo(occupied);
	}
	return objectState.interactibleTiles;
}

VTile Resource::size() {
	auto& objectState = states[state];
	return objectState.size;
}

const std::vector<std::string>& Resource::getInteractions() {
	auto& objectState = states[state];
	return objectState.interactions;
}

void Resource::interact(const std::shared_ptr<User>& user, int objectState, const std::string& interaction) {
	auto& object = states[state];
	if (state != objectState || !object.interactions.size())
		return;

	if (interaction == object.interactions[COLLECT])
		collect(user);
	else if (interaction == "Examine")
		examine(user);
}

const std::string& Resource::getName() {
	return states[state].name;
}
