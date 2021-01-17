#include "gameData.h"

GameData GameDataStorage::getGameData(double tickFraction) {
	int dataIndex = 0.9999 - tickFraction;
	while (tickFraction < 0)
		tickFraction += 1;
	if (data.size() == 0)
		return GameData();
	if (data.size() == 1)
		return data[0];
	auto oldTickData = data[dataIndex + 1];
	auto recentTickData = data[dataIndex];
	GameData res;
	for (auto id : recentTickData.playerIdToPositionIndex) {
		auto oldIndex = oldTickData.playerIdToPositionIndex[id.first];
		auto newPos = recentTickData.playerPositions[id.second];
		if (oldIndex) {
			auto oldPos = oldTickData.playerPositions[oldIndex];
			res.playerPositions.push_back(oldPos * (1 - tickFraction) + newPos * tickFraction);
		} else
			res.playerPositions.push_back(newPos);
	}
	return res;
}

void GameDataStorage::onGameTick(JSON& gameData) {
	GameData gData;
	storePositions(gameData["positions"], gData);
	data.insert(data.begin(), gData);
}

void GameDataStorage::storePositions(JSON& positions, GameData& gData) {
	auto _positions = positions.children;
	gData.playerPositions.push_back(VTile());
	for (auto data : _positions) {
		int id = data["id"].asInt();
		int x = data["x"].asDouble();
		int y = data["y"].asDouble();
		gData.playerIdToPositionIndex[id] = gData.playerPositions.size();
		gData.playerPositions.push_back(VTile(x, y));
	}
}
