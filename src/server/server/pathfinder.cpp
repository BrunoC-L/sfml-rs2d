#include "pathfinder.h"

std::vector<VTile> Pathfinder::pathfind(VTile a, std::vector<VTile> b, PathFindOption opt, Map* map) {
	if (opt == PathFindOption::NextTo)
		b = MovingPredicate::getNextTo(b);
	else if (opt == PathFindOption::NextToOrOnto) {
		std::vector<VTile> nextTo = MovingPredicate::getNextTo(b);
		for (const auto& n : nextTo)
			b.push_back(n);
	}
	for (auto t : b)
		if (a == t)
			return {};
	auto path = pathfind(a, b, map);
	if (!path.size())
		return {};
	reverse(begin(path), end(path));
	path.erase(path.begin());
	return path;
}

std::vector<VTile> Pathfinder::pathfind(VTile a, std::vector<VTile> b, Map* map) {
	static Pathfinder instance;
	std::unordered_map<VTile, VTile, VTileHash> parents;
	std::unordered_set<VTile, VTileHash> seen;
	std::vector<VTile> queue = { a };
	VTile current, best;
	float bestDistance = 1000;

	int step = 0;

	while (queue.size() && (step++) < 10000) {
		current = queue[0];
		queue.erase(queue.begin());
		for (auto target : b) {
			auto distance = MovingPredicate::distance(current, target);
			if (distance < bestDistance) {
				bestDistance = distance;
				best = current;
			}
		}
		if (bestDistance == 0)
			break;
		seen.insert(current);
		auto nexts = {
			current + VTile(1,  0),
			current + VTile(-1,  0),
			current + VTile(0,  1),
			current + VTile(0, -1),
			current + VTile(1,  1),
			current + VTile(-1,  1),
			current + VTile(1, -1),
			current + VTile(-1, -1),
		};
		for (auto next : nexts) {
			if (seen.count(next) || !MovingPredicate::canMoveFromTo(current, next, map))
				continue;
			seen.insert(next);
			parents[next] = current;
			queue.push_back(next);
		}
	}
	std::vector<VTile> path = { best };
	while (parents.count(best)) {
		best = parents[best];
		path.push_back(best);
	}
	return path;
}
