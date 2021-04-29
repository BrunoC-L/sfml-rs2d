#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "keyPressedEvent.h"
#include "mouseWheelEvent.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMiddleClickEvent.h"
#include "mouseMoveEvent.h"
#include "resizeEvent.h"
#include <tuple>
#include "Tilemap.h"

const int TILES_PER_CHUNK = 64;
const int PIXELS_PER_TILE = 32;
const int PIXELS_PER_TILE_ON_IMAGE = 4;

using ColoredTextRect = std::pair<sf::RectangleShape, sf::Text>;

class MapEditor {
private:
	sf::Font font;
	bool drawn = false;

	std::vector<std::function<void()>> buttonCallbacks;
	ColoredTextRect reloadButton;
	ColoredTextRect toggleWallsButton;
	// ColoredTextRect toggleResources;
	// ColoredTextRect highlightResources;
	// toggle monsters, toggle  item spawns, toggle NPCS

	sf::Texture map;
	sf::RectangleShape chunks[29][25];
	TileMap walls[29][25];
	bool displayWalls = true;

	EventObserver<ResizeEvent> resizeObserver;
	EventObserver<MouseWheelEvent> wheelObserver;
	EventObserver<MouseLeftClickEvent> leftClickObserver;
	EventObserver<MouseRightClickEvent> rightClickObserver;

	VPixel stretch = VPixel(1, 1);
	float zoom = 1;
	const VPixel startingSize = VPixel(1160, 1000);
	VPixel pos = startingSize / 2;
	int margin = 10;
	int spacing = 10;
	sf::Vector2f size = sf::Vector2f(200, 44);

	std::shared_ptr<sf::RenderWindow> window;
	void pollEvents();
	VPixel getSize();
	void draw();
	sf::Transform getMapTransform(const VTile& relativePos);
	void setupButtons();
	void setupMap();
	void setupObservers();
	void reload();
	void toggleWalls();
	std::string getWallsFileName(int x, int y, int z) const;
public:
	MapEditor();
	void run();
};
