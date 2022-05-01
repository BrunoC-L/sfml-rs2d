#pragma once
#include "units.h"
#include "json.h"
#include "abstractPlayer.h"
#include <vector>
#include <mutex>

using playerIdAndPosition = std::pair<int, VTile>;

struct playerIdAndPositions {
	int playerID;
	VTile oldPosition;
	VTile newPosition;
};

class PlayerPositions {
	std::pair<std::unique_ptr<std::vector<playerIdAndPosition>>, std::unique_ptr<std::vector<playerIdAndPosition>>> data;
	std::vector<playerIdAndPositions> compiledPositions;
	std::mutex mutex;
	VTile weightedAverage(VTile v1, VTile v2, double weight);
	int playerId;
	AbstractPlayer* player;
	void compile();
public:
	PlayerPositions(AbstractPlayer* player);
	std::vector<playerIdAndPosition> getPlayerPositions(double tickFraction);
	void update(std::vector<playerIdAndPosition> newPositions);
	void update(const JSON& json);
};
