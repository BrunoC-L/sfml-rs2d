#pragma once
#include "units.h"
#include "json.h"
#include <utility>
#include <vector>
#include <mutex>

using playerIdAndPosition = std::pair<int, VTile>;

class PlayerPositions {
	std::pair<std::unique_ptr<std::vector<playerIdAndPosition>>, std::unique_ptr<std::vector<playerIdAndPosition>>> data;
	std::mutex mutex;
	VTile weightedAverage(VTile v1, VTile v2, double weight);
public:
	PlayerPositions();
	std::vector<playerIdAndPosition> getPlayerPositions(double tickFraction);
	void update(std::vector<playerIdAndPosition> newPositions);
	void update(JSON& json);
};
