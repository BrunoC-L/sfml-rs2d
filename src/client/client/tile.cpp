#include "tile.h"

Tile::Tile(int x, int y) : position(x, y) {
}

void Tile::onLeftClick(MouseLeftClickEvent event) {
	WalkClickEvent(position).emit();
}

void Tile::onRightClick(MouseRightClickEvent event) {
	RightClickTileEvent(this).emit();
}

void Tile::onMiddleClick(MouseMiddleClickEvent event) {
	TeleportEvent(position).emit();
}
