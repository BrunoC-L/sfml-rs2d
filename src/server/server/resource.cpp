#include "resource.h"
#include "gameMessageEvent.h"

Resource::Resource(std::string&& fileName, JSON&& json, Tile* tile) : Object(fileName, tile), json(json) { }

void Resource::build() {
	const auto& objects = json.getChildren();
	states.reserve(objects.size());
	for (const auto& objectState : objects) {
		ObjectState state;
		state.calculatedInteractibleTiles = false;
		state.name = objectState.get("name").asString();
		state.examine = objectState.get("examine").asString();
		state.size = VTile(objectState.get("size").getChildren()[0].asInt(), objectState.get("size").getChildren()[1].asInt());
		for (const auto& i : objectState.get("interactions").getChildren())
			state.interactions.push_back(i.get("name").asString());
		states.push_back(state);
	}
}

void Resource::collect(const std::shared_ptr<User>& user) {
	if (state != 0)
		return;
	EVENT(GoToObjectRequestEvent, user, this, [user, this]() {
		interactors.push_back({ 0, user });
		if (!tickObserver.isSet())
			tickObserver.set([&](const TickEvent::Data& ev) { tick(); });
	}).emit();
	EVENT(SubscribeToInteractionInterruptionEvent, user, [&]() {
		/*auto it = std::find(interactors.begin(), interactors.end(), [&](const Interactor& i) { return i.user == user; });
		if (it != interactors.end())
			interactors.erase(it);*/
	}).emit();
}

void Resource::examine(const std::shared_ptr<User>& user) {
	auto& object = states[state];
	GameMessageEvent(GameMessageEventData{ user, object.examine }).emit();
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

const std::string& Resource::getName() {
	return states[state].name;
}

void Resource::setState(int state) {
	interactors = {};
	Object::setState(state);
}

void Resource::tick() {
	if (!interactors.size())
		tickObserver.unset();
	for (auto& i : interactors)
		tick(i);
}
