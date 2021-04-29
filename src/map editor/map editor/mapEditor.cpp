#include "mapEditor.h"
#include <fstream>

MapEditor::MapEditor(): window(std::make_shared<sf::RenderWindow>(sf::VideoMode(startingSize.x, startingSize.y), "RS2D Map Editor")) {
	setupMap();
	setupButtons();
	setupObservers();
	ResizeEvent().emit();
}

void MapEditor::run() {
	while (window->isOpen()) {
		pollEvents();
		if (drawn)
			continue;
		window->clear();
		draw();
		window->display();
	}
}

void MapEditor::draw() {
	drawn = true;
	VTile relativePos = (pos - getSize() / 2) / PIXELS_PER_TILE;
	sf::Transform transform = getMapTransform(relativePos);
	for (int x = 0; x < 29; ++x) {
		for (int y = 0; y < 25; ++y) {
			sf::Transform t(transform);
			t.translate(sf::Vector2f(TILES_PER_CHUNK * x, TILES_PER_CHUNK * y));
			t.scale(2.f / PIXELS_PER_TILE, 2.f / PIXELS_PER_TILE);
			window->draw(chunks[x][y], t);
			t.scale(0.5, 0.5);
			if (displayWalls && zoom > 10)
				walls[x][y].draw(*window, t);
		}
	}
	sf::Transform buttonTransform;
	buttonTransform.scale(1 / stretch.x, 1 / stretch.y);
	window->draw(reloadButton.first, buttonTransform);
	window->draw(reloadButton.second, buttonTransform);
	window->draw(toggleWallsButton.first, buttonTransform);
	window->draw(toggleWallsButton.second, buttonTransform);
}

void MapEditor::pollEvents() {
	sf::Event event;
	while (window->pollEvent(event))
		if (event.type == sf::Event::Closed)
			window->close();
		else if (event.type == sf::Event::KeyPressed)
			switch (event.text.unicode) {
			case 71:
				LeftArrowKeyPressedEvent().emit();
				break;
			case 72:
				RightArrowKeyPressedEvent().emit();
				break;
			case 58:
				EnterKeyPressedEvent().emit();
				break;
			case 59:
				BackspaceKeyPressedEvent().emit();
				break;
			case 60:
				TabKeyPressedEvent().emit();
				break;
			default:
				int code = event.text.unicode;
				bool isLetter = code < 26;
				if (isLetter) {
					LetterKeyPressedEvent(char(97 + code), false).emit();
					break;
				}
				bool isNumber = code < 36;
				if (isNumber) {
					LetterKeyPressedEvent(char(22 + code), false).emit();
					break;
				}
			}
		else if (event.type == sf::Event::MouseButtonPressed) {
			switch (event.mouseButton.button) {
			default:
			case Left:
				MouseLeftClickEvent(VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
				break;
			case Right:
				MouseRightClickEvent(VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
				break;
			case Middle:
				MouseMiddleClickEvent(VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
				break;
			}
		}
		else if (event.type == sf::Event::Resized)
			ResizeEvent().emit();
		else if (event.type == sf::Event::MouseWheelMoved)
			MouseWheelEvent(VPixel(event.mouseWheel.x, event.mouseWheel.y), event.mouseWheel.delta).emit();
		else if (event.type == sf::Event::MouseMoved)
			MouseMoveEvent(VPixel(event.mouseMove.x, event.mouseMove.y)).emit();
}

VPixel MapEditor::getSize() {
	VPixel v;
	auto size = window->getSize();
	v.x = size.x;
	v.y = size.y;
	return v;
}

sf::Transform MapEditor::getMapTransform(const VTile& relativePos) {;
	VTile offsetTiles = relativePos * -1;
	const auto offset = VPixel(PIXELS_PER_TILE * offsetTiles.x * zoom, PIXELS_PER_TILE * offsetTiles.y * zoom);
	const VTile scalingDiff = getSize() * VTile(1 - zoom, 1 - zoom);
	const VPixel scalingDiffPx = VPixel(scalingDiff.x, scalingDiff.y) / 2;
	const auto finalOffset = offset + scalingDiffPx;

	sf::Transform transform;
	transform.scale(sf::Vector2f(1 / stretch.x, 1 / stretch.y));
	transform.translate(sf::Vector2f(finalOffset.x, finalOffset.y));
	transform.scale(zoom, zoom);
	return transform;
}

void MapEditor::setupButtons() {
	font.loadFromFile("../../../assets/runescape_uf.ttf");

	auto pos = sf::Vector2f(margin, margin);
	int correctionYtext = 8;

	reloadButton.second.setString("Reload Objects");
	reloadButton.second.setFont(font);
	reloadButton.second.setFillColor(sf::Color::Green);
	reloadButton.second.setPosition(pos + sf::Vector2f(3, -4));
	reloadButton.second.setCharacterSize(35);
	reloadButton.first.setPosition(pos);
	reloadButton.first.setFillColor(sf::Color::Red);
	reloadButton.first.setSize(size);

	pos.y += size.y + spacing;
	toggleWallsButton.second.setString("Toggle Walls");
	toggleWallsButton.second.setFont(font);
	toggleWallsButton.second.setFillColor(sf::Color::Green);
	toggleWallsButton.second.setPosition(pos + sf::Vector2f(3, -4));
	toggleWallsButton.second.setCharacterSize(35);
	toggleWallsButton.first.setPosition(pos);
	toggleWallsButton.first.setFillColor(sf::Color::Red);
	toggleWallsButton.first.setSize(size);

	buttonCallbacks.reserve(2);
	buttonCallbacks.emplace_back([&]() {
		reload();
	});
	buttonCallbacks.emplace_back([&]() {
		toggleWalls();
	});
}

void MapEditor::setupMap() {
	map.loadFromFile("../../../assets/mapnoraids.jpg");

	for (int x = 0; x < 29; ++x) {
		for (int y = 0; y < 25; ++y) {
			std::cout << "Loading chunk " << x << ", " << y << std::endl;

			chunks[x][y].setTexture(&map);
			chunks[x][y].setTextureRect(
				sf::IntRect(
					sf::Vector2i(x * TILES_PER_CHUNK * PIXELS_PER_TILE_ON_IMAGE, y * TILES_PER_CHUNK * PIXELS_PER_TILE_ON_IMAGE),
					sf::Vector2i(    TILES_PER_CHUNK * PIXELS_PER_TILE_ON_IMAGE,     TILES_PER_CHUNK * PIXELS_PER_TILE_ON_IMAGE)
				)
			);
			chunks[x][y].setSize(
				sf::Vector2f(
					PIXELS_PER_TILE * PIXELS_PER_TILE,
					PIXELS_PER_TILE * PIXELS_PER_TILE
				)
			);

			int* wallTypes = new int[TILES_PER_CHUNK * TILES_PER_CHUNK]; // leak but we only ever do it once so doesn't matter

			std::string fileName = getWallsFileName(x, y, 0);
			std::ifstream file(fileName);
			std::string line;
			if (!file.is_open())
				throw std::exception(("Failed to open " + fileName).c_str());
			for (int tx = 0; tx < TILES_PER_CHUNK; ++tx) {
				for (int ty = 0; ty < TILES_PER_CHUNK; ++ty) {
					std::getline(file, line);
					const int borders = stoi(line);
					wallTypes[tx * TILES_PER_CHUNK + ty] = borders;
					// to display borders
					//wallTypes[tx * TILES_PER_CHUNK + ty] = (tx == 0 || ty == 0 || tx == 63 || ty == 63) ? 15 : borders;
				}
			}

			walls[x][y].load(
				"../../../assets/textures/red_walls.png",
				sf::Vector2u(PIXELS_PER_TILE, PIXELS_PER_TILE),
				wallTypes,
				TILES_PER_CHUNK,
				TILES_PER_CHUNK
			);
		}
	}
}

void MapEditor::setupObservers() {
	resizeObserver.set([&](ResizeEvent& ev) {
		float x = getSize().x / startingSize.x;
		float y = getSize().y / startingSize.y;
		stretch = VPixel(x, y);
		drawn = false;
	});

	wheelObserver.set([&](MouseWheelEvent& ev) {
		if (ev.delta == 0)
			return;
		zoom = zoom * (1 + (0.3 + ev.delta) * 0.1f);
		std::cout << ev.delta << std::endl;
		zoom = std::max(1.f, zoom);
		zoom = std::min(1000.f, zoom);
		drawn = false;
	});

	leftClickObserver.set([&](MouseLeftClickEvent& ev) {
		int left, right, top, bottom;
		left = margin;
		right = left + size.x;
		top = margin;
		bottom = top + size.y;
		for (int i = 0; i < buttonCallbacks.size(); ++i) {
			if ((ev.pos.x >= left - 1) && (ev.pos.x <= right + 1) && (ev.pos.y >= top - 1) && (ev.pos.y <= bottom + 1)) {
				buttonCallbacks[i]();
				return;
			}
			top = bottom + spacing;
			bottom = top + size.y;
		}
		VPixel delta = ev.pos - getSize() / 2;
		pos += delta / zoom;
		pos.x = std::max(pos.x, 0.f);
		pos.y = std::max(pos.y, 0.f);
		//pos.x = std::min(pos.x, getSize().x);
		//pos.y = std::min(pos.y, getSize().y);
		drawn = false;
	});
}

void MapEditor::reload() {
	std::cout << "Reloaded\n";
}

void MapEditor::toggleWalls() {
	displayWalls = !displayWalls;
	std::cout << "Toggled Walls to " << (displayWalls ? "true" : "false") << std::endl;
}

std::string MapEditor::getWallsFileName(int x, int y, int z) const {
	return "../../../assets/walls/" +
		std::to_string(x) + "-" + std::to_string(y) + "-" + std::to_string(z) + ".txt";
}
