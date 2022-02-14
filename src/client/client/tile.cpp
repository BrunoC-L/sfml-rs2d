#include "tile.h"

Tile::Tile(int x, int y) : position(x, y) {
}

void Tile::onLeftClick(MouseLeftClickEvent event) {
	EVENT(WalkClickEvent, position).emit();
}

void Tile::onRightClick(MouseRightClickEvent event) {
	EVENT(RightClickTileEvent, this).emit();
}

void Tile::onMiddleClick(MouseMiddleClickEvent event) {
	EVENT(TeleportEvent, position).emit();
}

const std::vector<ObjectInteractions>& Tile::getInteractions() {
	return interactions;
}

void Tile::addInteractions(ObjectInteractions oi) {
	this->interactions = {};
	this->interactions.emplace_back(std::move(oi));
}
