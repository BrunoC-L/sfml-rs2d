#pragma once
#include "abstractMap.h"
#include <unordered_map>
#include <unordered_set>
#include "movingPredicate.h"
using namespace std;

class Pathfinder {
public:
	static vector<VTile> pathfind(VTile a, vector<VTile> b, bool nextToInsteadOfOnto, AbstractMap* map) {
		if (nextToInsteadOfOnto)
			b = MovingPredicate::getNextTo(b);
		for (auto t : b)
			if (a == t)
				return {};
		auto path = _pathfind(a, b, map);
		if (!path.size())
			return {};
		reverse(begin(path), end(path));
		path.erase(path.begin());
		return path;
	}
private:
	static vector<VTile> _pathfind(VTile a, vector<VTile> b, AbstractMap* map) {
		static Pathfinder instance;
		unordered_map<VTile, VTile, VTileHash> parents;
		unordered_set<VTile, VTileHash> seen;
		vector<VTile> queue = {a};
		VTile current, best;
		float bestDistance = 1000;

		while (queue.size()) {
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
				current + VTile( 1,  0),
				current + VTile(-1,  0),
				current + VTile( 0,  1),
				current + VTile( 0, -1),
				current + VTile( 1,  1),
				current + VTile(-1,  1),
				current + VTile( 1, -1),
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
		vector<VTile> path = { best };
		while (parents.count(best)) {
			best = parents[best];
			path.push_back(best);
		}
		return path;
	}
};