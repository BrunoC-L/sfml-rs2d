#pragma once
#include "map.h"
#include <unordered_map>
#include <unordered_set>
#include "movingPredicate.h"
using namespace std;

class Pathfinder {
public:
	static vector<VTile> pathfind(VTile a, vector<VTile> b, bool nextToInsteadOfOnto) {
		if (nextToInsteadOfOnto)
			b = MovingPredicate::getNextTo(b);
		for (auto t : b)
			if (a == t)
				return {};
		auto path = _pathfind(a, b);
		if (!path.size())
			return {};
		reverse(begin(path), end(path));
		path.erase(path.begin());
		return path;
	}
private:
	static vector<VTile> _pathfind(VTile a, vector<VTile> b) {
		static Pathfinder instance;
		unordered_map<VTile, VTile, VTileHash> parents;
		unordered_set<VTile, VTileHash> seen;
		unsigned index = 0;
		vector<VTile> queue = {a};
		VTile current;
		seen.insert(current);
		bool currentIsValid = false;
		while (queue.size() > index) {
			current = queue[index++];
			if (MovingPredicate::tileIsInVector(current, b)) {
				currentIsValid = true;
				break;
			}
			seen.insert(current);
			auto nexts = {
				current + VTile( 0,  1),
				current + VTile(-1,  0),
				current + VTile( 0, -1),
				current + VTile( 1,  0),
				current + VTile( 1,  1),
				current + VTile(-1,  1),
				current + VTile( 1, -1),
				current + VTile(-1, -1),
			};
			for (auto next : nexts) {
				if (seen.count(next) || !MovingPredicate::canMoveFromTo(current, next))
					continue;
				seen.insert(next);
				parents[next] = current;
				queue.push_back(next);
			}
		}
		if (!currentIsValid)
			return {};
		vector<VTile> path = {current};
		while (parents.count(current)) {
			current = parents[current];
			path.push_back(current);
		}
		return path;
	}	
};