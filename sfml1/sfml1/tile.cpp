#include "tile.h"

Tile::Tile(int x, int y, int borders, const vector<GroundItem> groundItems, const vector<GroundObject> groundObjects, const vector<NPC> NPCs, const function<void(void)> callback)
	: position(x, y), borders(borders), groundItems(groundItems), groundObjects(groundObjects), NPCs(NPCs), callback(callback) {
}

bool Tile::canMoveFrom(Tile from) {
	VTile delta = from.position - position;
	const int mycase = 10 * delta.x + delta.y;
	const int EAST = 0b1000, NORTH = 0b0100, WEST = 0b0010, SOUTH = 0b0001;
	// borders: 0b1101 ->  east, north, !west, south
	// -1, -1     0, -1     1, -1		|	 -11,  -1,   9
	// -1,  0     0,  0     1,  0		|	 -10,   0,  10
	// -1,  1     0,  1     1,  1		|	  -9,   1,  11
	switch (mycase) {
		case   0: return true;
		case -11: return !(borders & (NORTH | WEST) || from.borders & (SOUTH | EAST));
		case -10: return !(borders & WEST || from.borders & EAST);
		case  -9: return !(borders & (SOUTH | WEST) || from.borders & (NORTH | EAST));
		case  -1: return !(borders & NORTH || from.borders & SOUTH);
		case  11: 
		case  10:
		case   9:
		case   1: return from.canMoveFrom(*this);
		default:
			return false;
	}
}