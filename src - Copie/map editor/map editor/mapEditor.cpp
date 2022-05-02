#include "mapEditor.h"
#include "splitString.h"

MapEditor::MapEditor() : window(std::make_shared<sf::RenderWindow>(sf::VideoMode(startingSize.x, startingSize.y), "RS2D Map Editor")) {
	keys = { {"walls", true}, {"objects", false} };
	setupMap();
	setupButtons();
	setupObservers();
	se = std::make_shared<SearchEngine>(window, font, drawn, stretch);
	EVENT(ResizeEvent).emit();
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
			if (zoom > 5) {
				t.scale(0.5, 0.5);
				for (const auto& key : keys)
					if (displays.find(key.first) != displays.end())
						values[key.first][x][y].draw(*window, t);
			}
		}
	}
	littleSquare.setPosition(pos.x, pos.y);
	window->draw(littleSquare, transform);
	sf::Transform buttonTransform;
	buttonTransform.scale(1 / stretch.x, 1 / stretch.y);
	for (auto& button : leftButtons) {
		window->draw(button.first, buttonTransform);
		window->draw(button.second, buttonTransform);
	}
	se->draw();
}

void MapEditor::pollEvents() {
	sf::Event event;
	while (window->pollEvent(event))
		if (event.type == sf::Event::Closed)
			window->close();
		else if (event.type == sf::Event::KeyPressed)
			switch (event.text.unicode) {
			case 71:
				EVENT(LeftArrowKeyPressedEvent).emit();
				break;
			case 72:
				EVENT(RightArrowKeyPressedEvent).emit();
				break;
			case 58:
				EVENT(EnterKeyPressedEvent).emit();
				break;
			case 59:
				EVENT(BackspaceKeyPressedEvent).emit();
				break;
			case 60:
				EVENT(TabKeyPressedEvent).emit();
				break;
			default:
				int code = event.text.unicode;
				bool isLetter = code < 26;
				if (isLetter) {
					EVENT(LetterKeyPressedEvent, char(97 + code), false).emit();
					break;
				}
				bool isNumber = code < 36;
				if (isNumber) {
					EVENT(LetterKeyPressedEvent, char(22 + code), false).emit();
					break;
				}
			}
		else if (event.type == sf::Event::MouseButtonPressed) {
			switch (event.mouseButton.button) {
			default:
			case Left:
				EVENT(MouseLeftClickEvent, VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
				break;
			case Right:
				EVENT(MouseRightClickEvent, VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
				break;
			case Middle:
				EVENT(MouseMiddleClickEvent, VPixel(event.mouseButton.x, event.mouseButton.y)).emit();
				break;
			}
		}
		else if (event.type == sf::Event::Resized)
			EVENT(ResizeEvent).emit();
		else if (event.type == sf::Event::MouseWheelMoved) 
			EVENT(MouseWheelEvent, VPixel(event.mouseWheel.x, event.mouseWheel.y), event.mouseWheel.delta).emit();
		else if (event.type == sf::Event::MouseMoved)
			EVENT(MouseMoveEvent, VPixel(event.mouseMove.x, event.mouseMove.y)).emit();
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
		[&]() {toggle("walls"); },
		[&]() {toggle("objects"); },
		[&]() {save(); }
	};

	std::vector<std::string> leftButtonTexts = {
		"Toggle Walls",
		"Toggle Objects",
		"Save"
	};

	_ASSERT(leftButtonTexts.size() == leftButtonCallbacks.size());

	leftButtons.reserve(leftButtonCallbacks.size());

	for (auto& buttonText : leftButtonTexts) {
		leftButtons.push_back(std::pair<sf::RectangleShape, sf::Text>());
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
	loadTexture();
	for (const auto& key : keys)
		load(key);
	std::cout << "\rDone Loading Map                                         \n";
	littleSquare.setFillColor(sf::Color::Cyan);
	littleSquare.setSize(sf::Vector2f(1, 1));
	littleSquare.setOrigin(sf::Vector2f(0.5, 0.5));
}

void MapEditor::setupObservers() {
	resizeObserver.set([&](const ResizeEvent::Data& ev) {
		float x = getSize().x / startingSize.x;
		float y = getSize().y / startingSize.y;
		stretch = VPixel(x, y);
		drawn = false;
	});

	wheelObserver.set([&](const MouseWheelEvent::Data& ev) {
		if (ev.pos.y > window->getSize().y - 300 * stretch.y)
			return;
		std::cout << "remember to fix this\n";
		/*if (ev.delta == 0)
			ev.delta = scroll;
		else
			scroll = ev.delta;*/
		zoom = zoom * (1 + ev.delta * 0.05f);
		zoom = std::max(0.1f, zoom);
		zoom = std::min(1000.f, zoom);
		drawn = false;
	});

	leftClickObserver.set([&](const MouseLeftClickEvent::Data& ev) {
		if (ev.pos.y > window->getSize().y - 300 * stretch.y)
			return;
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
		VChunk chunk;
		VTile p;
		chunk.x = int(pos.x / TILES_PER_CHUNK);
		chunk.y = int(pos.y / TILES_PER_CHUNK);
		chunk.z = int(pos.z);
		p.x = int(pos.x) % TILES_PER_CHUNK;
		p.y = int(pos.y) % TILES_PER_CHUNK;
		currentFile = std::make_shared<editor::FileEditor>(chunk, p, walls[int(chunk.x)][int(chunk.y)][int(p.x * TILES_PER_CHUNK + p.y)]);
		se->reset();
		se->set(currentFile->values);
		drawn = false;
	});
}

void MapEditor::loadTexture() {
	for (int x = 0; x < 29; ++x) {
		for (int y = 0; y < 25; ++y) {
			chunks[x][y].setTexture(&map);
			chunks[x][y].setTextureRect(
				sf::IntRect(
					sf::Vector2i(x * TILES_PER_CHUNK * PIXELS_PER_TILE_ON_MAP, y * TILES_PER_CHUNK * PIXELS_PER_TILE_ON_MAP),
					sf::Vector2i(TILES_PER_CHUNK * PIXELS_PER_TILE_ON_MAP, TILES_PER_CHUNK * PIXELS_PER_TILE_ON_MAP)
				)
			);
			chunks[x][y].setSize(
				sf::Vector2f(
					PIXELS_PER_TILE * PIXELS_PER_TILE,
					PIXELS_PER_TILE * PIXELS_PER_TILE
				)
			);
		}
	}
}

void MapEditor::toggle(std::string key) {
	if (displays.find(key) != displays.end())
		displays.erase(key);
	else
		displays.insert(key);
}

void MapEditor::save() {
	if (currentFile)
		currentFile->save(se->getAdded());
}

void MapEditor::load(std::pair<std::string, bool> key) {
	std::cout << "Loading " << key.first << std::endl;
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile("../../../assets/textures-32/" + key.first + ".png"))
		throw std::exception(("Failed to open ../../../assets/textures-32/" + key.first + ".png").c_str());
	std::unordered_map<std::string, int> name2texture;
	std::unordered_map<std::string, int> name2count;
	if (!key.second) {
		std::ifstream name2textureIndex("../../../resource/" + key.first + "/" + key.first.substr(0, key.first.length() - 1) + "Name2texture.txt");
		if (!name2textureIndex.is_open())
			throw std::exception(("Failed to open ../../../resource/" + key.first + "/" + key.first.substr(0, key.first.length() - 1) + "Name2texture.txt").c_str());
		std::string nameOfTexture;
		while (std::getline(name2textureIndex, nameOfTexture)) {
			auto content = split(nameOfTexture, ":");
			name2count[content[0]] += 1;
			name2texture[content[0] + content[1]] = stoi(content[2]);
		}
	}
	for (int x = 0; x < 29; ++x) {
		for (int y = 0; y < 25; ++y) {
			std::cout << "\rLoading chunk " << x << ", " << y << "   "; // note the \r and extra spaces

			int* grid = new int[TILES_PER_CHUNK * TILES_PER_CHUNK](); // leak but we only ever do it once so doesn't matter
			if (walls[x][y] == nullptr)
				walls[x][y] = grid; // walls HAVE to be calculated before other tilemaps!
			std::string fileName = getFileName(key.first, x, y, 0);
			std::ifstream file(fileName);
			std::string line;
			if (!file.is_open())
				throw std::exception(("Failed to open " + fileName).c_str());
			if (key.second) {
				if (std::getline(file, line)) {
					std::vector<std::string> content = split(line, " ");
					if (content.size() == 1) {
						int mode = stoi(content[0]);
						std::fill_n(grid, TILES_PER_CHUNK * TILES_PER_CHUNK, mode);
					}
					else {
						const int tx = stoi(content[0]), ty = stoi(content[1]);
						grid[tx * TILES_PER_CHUNK + ty] = stoi(content[2]);
					}
				}
				while (std::getline(file, line)) {
					std::vector<std::string> content = split(line, ":");
					std::vector<std::string> x_y = split(content[0], "-");
					const int tx = stoi(x_y[0]), ty = stoi(x_y[1]);
					grid[tx * TILES_PER_CHUNK + ty] = stoi(content[1]);
				}
			}
			else {
				while (std::getline(file, line)) {
					std::vector<std::string> content = split(line, ":");
					auto x_y = split(content[0], "-");
					const int tx = stoi(x_y[0]), ty = stoi(x_y[1]);
					JSON json(content[1]);
					if (json.getChildren().size() == 0)
						continue;
					auto name = json.getChildren()[0].asString() + "-0";
					int count = name2count[name];
					int s = count == 9 ? 3 : count == 4 ? 2 : 1;
					for (int i = 0; i < count; ++i) {
						int idx = i % s, idy = i / s;
						auto txtname = name + " " + std::to_string(idx) + "-" + std::to_string(idy);
						grid[(tx + idx) * TILES_PER_CHUNK + (ty + idy)] = name2texture[txtname];
					}
				}
			}
			file.close();

			values[key.first][x][y].load(
				texture,
				sf::Vector2u(PIXELS_PER_TILE, PIXELS_PER_TILE),
				grid,
				TILES_PER_CHUNK,
				TILES_PER_CHUNK
			);
		}
	}
	std::cout << "\rdone loading " << key.first << "               \n";
}

std::string MapEditor::getFileName(std::string key, int x, int y, int z) const {
	return "../../../resource/chunks-64/" + key + "/" +
		std::to_string(x) + "-" + std::to_string(y) + "-" + std::to_string(z) + ".txt";
}
