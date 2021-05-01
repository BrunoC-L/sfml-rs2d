#include "mapEditor.h"

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
			if (displayWalls && zoom > 5)
				walls[x][y].draw(*window, t);
		}
	}
	sf::Transform buttonTransform;
	buttonTransform.scale(1 / stretch.x, 1 / stretch.y);
	for (auto& button : leftButtons)
		window->draw(button.second, buttonTransform);
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

	leftButtonCallbacks = {
		[&]() {reload(); },
		[&]() {toggleWalls(); },
		[&]() {toggleObjects(); },
		[&]() {save(); }
	};

	std::vector<std::string> leftButtonTexts = { "Reload", "Toggle Walls", "Toggle Objects", "Save" };

	_ASSERT(leftButtonTexts.size() == leftButtonCallbacks.size());

	leftButtons.reserve(leftButtonCallbacks.size());

	for (auto& buttonText : leftButtonTexts) {
		leftButtons.push_back(ColoredTextRect());
		auto& button = leftButtons.back();
		button.second.setString(buttonText);
		button.second.setFont(font);
		button.second.setFillColor(sf::Color::Green);
		button.second.setPosition(pos + sf::Vector2f(3, -4));
		button.second.setCharacterSize(35);
		button.first.setPosition(pos);
		button.first.setFillColor(sf::Color::Red);
		button.first.setSize(size);
		pos.y += size.y + spacing;
	}
}

void MapEditor::setupMap() {
	map.loadFromFile("../../../assets/mapnoraids.jpg");

	for (int x = 0; x < 29; ++x) {
		for (int y = 0; y < 25; ++y) {
			std::cout << "\rLoading chunk " << x << ", " << y << "   ";

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
			file.close();

			walls[x][y].load(
				"../../../assets/textures/red_walls.png",
				sf::Vector2u(PIXELS_PER_TILE, PIXELS_PER_TILE),
				wallTypes,
				TILES_PER_CHUNK,
				TILES_PER_CHUNK
			);
		}
	}
	std::cout << "\rDone Loading Map        \n";
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
			ev.delta = scroll;
		else
			scroll = ev.delta;
		zoom = zoom * (1 + ev.delta * 0.05f);
		zoom = std::max(0.1f, zoom);
		zoom = std::min(1000.f, zoom);
		drawn = false;
	});

	leftClickObserver.set([&](MouseLeftClickEvent& ev) {
		int left, right, top, bottom;
		left = margin;
		right = left + size.x;
		top = margin;
		bottom = top + size.y;
		for (int i = 0; i < leftButtonCallbacks.size(); ++i) {
			if ((ev.pos.x >= left - 1) && (ev.pos.x <= right + 1) && (ev.pos.y >= top - 1) && (ev.pos.y <= bottom + 1)) {
				leftButtonCallbacks[i]();
				drawn = false;
				return;
			}
			top = bottom + spacing;
			bottom = top + size.y;
		}
		VPixel delta = ev.pos - getSize() / 2;
		pos += delta / zoom;
		pos.x = (int)pos.x + 0.5;
		pos.y = (int)pos.y + 0.5;
		drawn = false;
	});

	rightClickObserver.set([&](MouseRightClickEvent& ev) {
		MouseLeftClickEvent(ev.pos).emit();
		currentFile = std::make_shared<editor::FileEditor>(pos);
	});
}

void MapEditor::reload() {

}

void MapEditor::toggleWalls() {
	displayWalls = !displayWalls;
}

void MapEditor::toggleObjects() {

}

void MapEditor::save() {
	if (currentFile)
		currentFile->save();
}

std::string MapEditor::getWallsFileName(int x, int y, int z) const {
	return "../../../assets/walls/" +
		std::to_string(x) + "-" + std::to_string(y) + "-" + std::to_string(z) + ".txt";
}
