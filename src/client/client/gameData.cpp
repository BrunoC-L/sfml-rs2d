#include "gameData.h"

GameData GameDataStorage::getGameData(double tickFraction) {
	if (tickFraction >= 1)
		tickFraction = 1;
	int dataIndex = 1 - tickFraction;
	while (tickFraction < 0)
		tickFraction += 1;
	if (data.size() == 0)
		return GameData();
	if (data.size() == 1)
		return data[0];
	if (cache.first != dataIndex) {
		cache.first = dataIndex;
		if (data.size() > dataIndex + 1)
			data.erase(data.begin() + dataIndex + 2, data.end());
		auto& oldTickData = data[dataIndex + 1];
		auto& recentTickData = data[dataIndex];
		GameData d1, d2;
		for (auto id : recentTickData.playerIdToPositionIndex) {
			auto newPos = recentTickData.playerPositions[id.second];
			d2.playerPositions.push_back(newPos);

			auto oldIndex = oldTickData.playerIdToPositionIndex[id.first];
			if (oldIndex) {
				auto oldPos = oldTickData.playerPositions[oldIndex];
				d1.playerPositions.push_back(oldPos);
			}
			else
				d1.playerPositions.push_back(newPos);
		}
		cache.second = { d1, d2 };
	}
	auto& v1 = cache.second.first.playerPositions;
	auto& v2 = cache.second.second.playerPositions;
	int n = v1.size();
	GameData res;
	res.playerPositions = std::vector<VTile>(n);
	for (int i = 0; i < n; ++i)
		res.playerPositions[i] = v1[i] * (1 - tickFraction) + v2[i] * tickFraction;
	return res;
}

void GameDataStorage::onGameTick(JSON& gameData) {
	GameData gData;
	storePositions(gameData["positions"], gData);
	data.insert(data.begin(), gData);
	cache.first += 1;
}

void GameDataStorage::storePositions(JSON& positions, GameData& gData) {
	auto _positions = positions.children;
	gData.playerPositions.push_back(VTile());
	for (int i = 0; i < _positions.size(); ++i) {
		auto& data = _positions[i];
		int id = data["id"].asInt();
		int x = data["x"].asDouble();
		int y = data["y"].asDouble();
		gData.playerIdToPositionIndex[id] = i;
		gData.playerPositions.push_back(VTile(x, y));
	}
}
