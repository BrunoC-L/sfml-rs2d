#include "PlayerPositions.h"

PlayerPositions::PlayerPositions() {
	data.first = std::make_unique<std::vector<playerIdAndPosition>>();
	data.second = std::make_unique<std::vector<playerIdAndPosition>>();
}

VTile PlayerPositions::weightedAverage(VTile v1, VTile v2, double weight) {
	return VTile(v1 * (1 - weight) + v2 * weight);
}

std::vector<playerIdAndPosition> PlayerPositions::getPlayerPositions(double tickFraction) {
	std::lock_guard<std::mutex> lock(mutex);
	if (data.first.get()->size() == 0)
		return *(data.second.get());
	std::vector<playerIdAndPosition> result;
	auto& oldpositions = *(data.first.get()), newpositions = *(data.second.get());
	int l1 = oldpositions.size(), l2 = newpositions.size();
	int i = 0, j = 0;
	while (i < l1 && j < l2) {
		auto& p1 = oldpositions[i], p2 = newpositions[i];
		if (p1.first == p2.first) {
			result.push_back(playerIdAndPosition(p1.first, weightedAverage(p1.second, p2.second, tickFraction)));
			++i;
			++j;
		}
		else if (p2.first < p1.first) { // new player, wasnt there previous tick, we just have the most recent data
			result.push_back(p2);
			++j;
		}
		else
			++i; // old player, is not in most recent data, keep going
	}
	while (j < l2) {
		auto& p2 = newpositions[i];
		result.push_back(p2);
		++j;
	}
	return result;
}

void PlayerPositions::update(std::vector<playerIdAndPosition> newPositions) {
	std::lock_guard<std::mutex> lock(mutex);
	data.first  = std::move(data.second);
	data.second = std::make_unique<std::vector<playerIdAndPosition>>(newPositions);
}

void PlayerPositions::update(JSON& json) {
	auto positions = json.children;
	std::vector<playerIdAndPosition> newPositions;
	for (auto& pos : positions) {
		auto x = pos["x"].asInt();
		auto y = pos["y"].asInt();
		auto id = pos["id"].asInt();
		auto tile = VTile(x, y);
		newPositions.push_back(playerIdAndPosition(id, tile));
	}
	update(newPositions);
}
