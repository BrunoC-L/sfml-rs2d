#include "rock.h"
#include "gameMessageEvent.h"

void Rock::build() {
	states.reserve(2);
	int size = json.get("size").asInt();
	int requirement = json.get("level requirement").asInt();
	int lives = json.get("lives").asInt();
	std::vector<std::string> interactions[2] = { { "Mine", "Prospect" }, { "Prospect" } };
	prospects[0] = json.get("prospects").getChildren()[0].asString();
	prospects[1] = json.get("prospects").getChildren()[1].asString();
	for (int i = 0; i < 2; ++i) {
		ObjectState state;
		state.name = "Rock";
		state.examine = "A rock.";
		state.size = VTile(size, size);
		state.interactions = std::move(interactions[i]);
		state.calculatedInteractibleTiles = false;
		states.emplace_back(state);
	}
}

void Rock::interact(const std::shared_ptr<User>& user, int objectState, const std::string& interaction) {
	auto& object = states[state];
	if (state != objectState)
		return;

	if (interaction == "Examine")
		return examine(user);

	if (state == 0 && interaction == "Mine")
		collect(user);

	if (interaction == "Prospect")
		prospect(user);
}

void Rock::prospect(const std::shared_ptr<User>& user) {
	GoToObjectRequestEvent(GoToObjectRequestEventData{ user, this, [user, this]() {
		interactors.push_back({ 1, user });
		if (!tickObserver.isSet())
			tickObserver.set([&](const TickEventData& ev) { Resource::tick(); });
	} }).emit();
	SubscribeToInteractionInterruptionEvent(SubscribeToInteractionInterruptionEventData{ user, [&]() {
		auto it = std::find(interactors.begin(), interactors.end(), std::pair<int, std::shared_ptr<User>>(1, user));
		if (it != interactors.end())
			interactors.erase(it);
	} }).emit();
}

void Rock::tick(int interactionId, const std::shared_ptr<User>& user) {
	if (interactionId == 0) {
		if (state == 1)
			return;
		if (rollPercent(30)) {
			setState(1);
			EVENT(ScheduleTaskEvent, randint(4, 6), [&]() { setState(0); }).emit();
		}
	}
	else {
		GameMessageEvent(GameMessageEventData{ user, prospects[state] }).emit();
		auto it = std::find(interactors.begin(), interactors.end(), std::pair<int, std::shared_ptr<User>>(1, user));
		if (it != interactors.end())
			interactors.erase(it);
	}
}

void Rock::setState(int state) {
	std::vector<std::pair<int, std::shared_ptr<User>>> out;
	std::copy_if(interactors.begin(), interactors.end(), std::back_inserter(out), [&](const std::pair<int, std::shared_ptr<User>>& pair) { return pair.first == 1; });
	interactors = out;
	Object::setState(state);
}
