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
#include <unordered_set>
#include "searchEngine.h"

class MapEditor {
private:
	sf::RectangleShape littleSquare;

	sf::Font font;
	bool drawn = false;

	std::vector<std::function<void()>> leftButtonCallbacks;
	std::vector<std::pair<sf::RectangleShape, sf::Text>> leftButtons;
	std::vector<std::function<void()>> rightButtonCallbacks;
	std::vector<std::pair<sf::RectangleShape, sf::Text>> rightButtons;
	void toggle(std::string key);
	void save();

	sf::Texture map;
	int* walls[29][25]{ nullptr };
	sf::RectangleShape chunks[29][25];
	std::unordered_map<std::string, TileMap[29][25]> values;
	std::unordered_set<std::string> displays;
	std::vector<std::pair<std::string, bool>> keys;

	EventObserver<ResizeEvent> resizeObserver;
	EventObserver<MouseWheelEvent> wheelObserver;
	EventObserver<MouseLeftClickEvent> leftClickObserver;

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
	void loadTexture();
	void load(std::pair<std::string, bool> key);
	std::string getFileName(std::string key, int x, int y, int z) const;

	std::shared_ptr<editor::FileEditor> currentFile;
	std::shared_ptr<SearchEngine> se;
public:
	MapEditor();
	void run();
};
