#include "tree.h"

void Tree::build() {
	states.reserve(2);
	int size = json.get("size").asInt();
	int requirement = json.get("level requirement").asInt();
	int lives = json.get("lives").asInt();
	std::vector<std::string> interactions[2] = { {"Chop"}, {} };
	for (int i = 0; i < 2; ++i) {
		ObjectState state;
		state.name = json.get("names").getChildren()[i].asString();
		state.examine = json.get("examines").getChildren()[i].asString();
		state.size = VTile(size, size);
		state.interactions = std::move(interactions[i]);
		state.calculatedInteractibleTiles = false;
		states.emplace_back(state);
	}
}

void Tree::interact(const std::shared_ptr<User>& user, int objectState, const std::string& interaction) {
	auto& object = states[state];
	if (state != objectState)
		return;

	if (interaction == "Examine")
		return examine(user);

	if (state == 0 && interaction == "Chop")
		collect(user);
}

void Tree::tick(int i, const std::shared_ptr<User>& user) {
	if (state == 1)
		return;
	if (rollPercent(30)) {
		setState(1);
		ScheduleTaskEvent(randint(4, 6), [&]() { setState(0); }).emit();
	}
}
