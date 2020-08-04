#pragma once
#include "map.h"
#include <unordered_map>
#include <unordered_set>
using namespace std;

class Pathfinder {
public:
	static vector<VTile> pathfind(VTile a, VTile b, function<bool(VTile, VTile)> canMoveFromTo) {
		if (a == b)
			return {};
		auto path = _pathfind(a, b, canMoveFromTo);
		if (path.size() == 0)
			return path;
		reverse(begin(path), end(path));
		vector<VTile> skipping = {};
		for (int i = 2; i < path.size(); i += 2)
			skipping.push_back(path[i]);
		if (!(path.size() % 2))
			skipping.push_back(path[path.size() - 1]);
		return skipping;
	}
private:
	static vector<VTile> _pathfind(VTile a, VTile b, function<bool(VTile, VTile)> canMoveFromTo) {
		unordered_map<VTile, VTile, VTileHash> parents;
		unordered_set<VTile, VTileHash> seen;
		unsigned index = 0;
		vector<VTile> queue = {a};
		VTile current;
		seen.insert(current);
		while (queue.size() != index) {
			current = queue[index++];
			if (current == b)
				break;
			seen.insert(current);
			auto nexts = {
				current + VTile( 0,  1),
				current + VTile( 1,  0),
				current + VTile( 0, -1),
				current + VTile(-1,  0),
				current + VTile( 1,  1),
				current + VTile(-1,  1),
				current + VTile( 1, -1),
				current + VTile(-1, -1),
			};
			for (auto next : nexts) {
				if (seen.count(next) || !canMoveFromTo(current, next))
					continue;
				seen.insert(next);
				parents[next] = current;
				queue.push_back(next);
			}
		}
		if (current != b)
			return {};
		vector<VTile> path = {current};
		while (parents.count(current)) {
			current = parents[current];
			path.push_back(current);
		}
		return path;
	}	
};