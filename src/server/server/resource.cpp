#include "resource.h"
#include "tile.h"
#include "movingPredicate.h"
#include "goToObjectRequestEvent.h"

Resource::Resource(JSON&& json, Tile* tile) : Object(std::move(json), tile), state(0) {
	for (const auto& objectState : this->json.getChildren())
		std::cout << objectState.asString(4);
}

void Resource::collect(const std::shared_ptr<User>& user) {
	GoToObjectRequest(user, this, [&]() {
		std::cout << "Hello World!\n";
	}).emit();
}

bool Resource::requirementsMet(const std::shared_ptr<User>& user)
{
	return false;
}

void Resource::giveExperience(const std::shared_ptr<User>& user)
{
}

const std::vector<VTile>& Resource::getInteractibleTiles() {
	if (!calculatedTiles) {
		calculatedTiles = true;
		std::vector<VTile> occupied = {};
		const auto size = this->size();
		for (int x = 0; x < size.x; ++x)
			for (int y = 0; y < size.y; ++y)
				occupied.push_back(getTile()->position + VTile(x, y));
		interactibleTiles = MovingPredicate::getNextTo(occupied);
	}
	return interactibleTiles;
}

VTile Resource::size() {
	return VTile(2, 2);
}

const std::vector<std::string>& Resource::getInteractions() {
	return interactions[state];
}

void Resource::interact(const std::shared_ptr<User>& user, int objectState, int index) {
	if (state != objectState)
		return;
	if (state == 0 && index == 0)
		collect(user);
	if (index == 1)
		std::cout << examines[state];
}

const std::string& Resource::getName() {
	return names[state];
}

int Resource::getState() {
	return state;
}
