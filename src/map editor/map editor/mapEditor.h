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
#include "fileEditor.h"

using ColoredTextRect = std::pair<sf::RectangleShape, sf::Text>;

class MapEditor {
private:
	sf::Font font;
	bool drawn = false;

	std::vector<std::function<void()>> leftButtonCallbacks;
	std::vector<ColoredTextRect> leftButtons;
	std::vector<std::function<void()>> rightButtonCallbacks;
	std::vector<ColoredTextRect> rightButtons;
	void reload();
	void toggleWalls();
	void toggleObjects();
	void save();

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
	int margin = 10;
	int spacing = 10;
	sf::Vector2f size = sf::Vector2f(200, 44);
	int scroll = 0;
	VPixel pos = VTile(1172.5, 869.5, 0);

	std::shared_ptr<sf::RenderWindow> window;
	void pollEvents();
	VPixel getSize();
	void draw();
	sf::Transform getMapTransform(const VTile& relativePos);
	void setupButtons();
	void setupMap();
	void setupObservers();
	std::string getWallsFileName(int x, int y, int z) const;
	std::shared_ptr<editor::FileEditor> currentFile;
public:
	MapEditor();
	void run();
};
