#pragma once
#include <vector>
#include <unordered_map>
#include "units.h"
#include "json.h"

class GameData {
public:
	std::vector<VTile> playerPositions;
	std::unordered_map<int, int> playerIdToPositionIndex;
};

class GameDataStorage {
public:
	std::vector<GameData> data;
	GameDataStorage() = default;
	GameData getGameData(double tickFraction);
	void onGameTick(JSON& gameData);
	void storePositions(JSON& positions, GameData& data);
};
