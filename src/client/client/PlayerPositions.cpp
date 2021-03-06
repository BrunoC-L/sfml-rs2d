#include "PlayerPositions.h"

PlayerPositions::PlayerPositions(AbstractPlayer* player) : player(player), playerId(player->getID()) {
	data.first = std::make_unique<std::vector<playerIdAndPosition>>();
	data.second = std::make_unique<std::vector<playerIdAndPosition>>();
}

VTile PlayerPositions::weightedAverage(VTile v1, VTile v2, double weight) {
	return VTile(v1 * (1 - weight) + v2 * weight);
}

std::vector<playerIdAndPosition> PlayerPositions::getPlayerPositions(double tickFraction) {
	std::lock_guard<std::mutex> lock(mutex);
	std::vector<playerIdAndPosition> result;
	for (const auto& p : compiledPositions) {
		auto pos = weightedAverage(p.oldPosition, p.newPosition, tickFraction);
		result.emplace_back(p.playerID, pos);
		if (p.playerID == playerId)
			player->setPosition(pos);
	}
	return result;
}

void PlayerPositions::update(std::vector<playerIdAndPosition> newPositions) {
	std::lock_guard<std::mutex> lock(mutex);
	data.first  = std::move(data.second);
	data.second = std::make_unique<std::vector<playerIdAndPosition>>(newPositions);
	compile();
}

void PlayerPositions::update(const JSON& json) {
	auto& positions = json.getChildren();
	std::vector<playerIdAndPosition> newPositions;
	for (auto& pos : positions) {
		auto x = pos.get("x").asInt();
		auto y = pos.get("y").asInt();
		auto id = pos.get("id").asInt();
		auto pos = VTile(x, y);
		newPositions.push_back(playerIdAndPosition(id, pos));
		if (id == playerId)
			player->setIntPosition(pos);
	}
	update(newPositions);
}

void PlayerPositions::compile() {
	compiledPositions = {};
	auto& oldpositions = *(data.first.get()), newpositions = *(data.second.get());
	int l1 = oldpositions.size(), l2 = newpositions.size();
	int i = 0, j = 0;
	while (i < l1 && j < l2) {
		auto& p1 = oldpositions[i];
		auto& p2 = newpositions[j];
		if (p1.first == p2.first) {
			compiledPositions.push_back({ p1.first, p1.second, p2.second });
			++i;
			++j;
		}
		else if (p2.first < p1.first) { // new player, wasnt there previous tick, we just have the most recent data
			compiledPositions.push_back({ p2.first, p2.second, p2.second });
			++j;
		}
		else {
			++i; // old player, is not in most recent data, keep going
		}
	}
	while (j < l2) {
		auto& p2 = newpositions[j];
		compiledPositions.push_back({ p2.first, p2.second, p2.second });
		++j;
	}
}
