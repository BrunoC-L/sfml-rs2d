#include "sfRenderWindow.h"
#include "keyPressedEvent.h"
#include "resizeEvent.h"
#include "frameEvent.h"

SFRenderWindow::SFRenderWindow(
	AbstractServiceProvider* provider,
	sf::RenderWindow& window
) : Service(provider),
	converter(provider),
	window(window) { }

void SFRenderWindow::init() {
	acquire();
	setFramerateLimit(60);
	updateWindowSize();

	rightBanner = new RightBanner(provider, this);
	bottomBanner = new BottomBanner(provider, this);
	rightClickInterface = new RightClickInterface(provider, this);

	p_t.loadFromFile("../../../assets/player.png");
	playerSprite = sf::Sprite(p_t);

	loginPage = sf::RectangleShape(sf::Vector2f(measures->windowSize.x, measures->windowSize.y));
	loginTexture.loadFromFile("../../../assets/login.png");
	loginPage.setTexture(&loginTexture);

	MouseLeftClickEvent::subscribe(new EventObserver<MouseLeftClickEvent>([&](MouseLeftClickEvent* ev) {
		if (gameData->userIsLoggedIn()) {
			bool clickedOnRightClickInterface = rightClickInterface->active && rightClickInterface->mouseIsInRect(ev);
			if (clickedOnRightClickInterface) {
				rightClickInterface->click(ev);
				return;
			}
			bool clickedOnRightBanner = rightBanner->mouseIsInRect(ev);
			if (clickedOnRightBanner) {
				rightBanner->click(ev);
				return;
			}
			bool clickedOnBottomBanner = bottomBanner->mouseIsInRect(ev);
			if (clickedOnBottomBanner) {
				bottomBanner->click(ev);
				return;
			}
			VTile tileClicked = converter.getPositionInGame(ev->pos);
			Tile* t = map->getTileFromVTile(tileClicked);
			if (t)
				t->onLeftClick(*ev);
		}
		else {
			auto abs = ev->pos / measures->stretch;
			auto newUserButton = std::make_pair(std::make_pair<int>(407, 659), std::make_pair<int>(495, 565));
			bool clickedOnNewUser =
				abs.x > newUserButton.first.first &&
				abs.x < newUserButton.first.second &&
				abs.y > newUserButton.second.first &&
				abs.y < newUserButton.second.second;
			if (clickedOnNewUser) {
				player->signUp();
				return;
			}
			auto existingUserButton = std::make_pair(std::make_pair<int>(690, 942), std::make_pair<int>(495, 565));
			bool clickedOnExistingUser =
				abs.x > existingUserButton.first.first &&
				abs.x < existingUserButton.first.second&&
				abs.y > existingUserButton.second.first &&
				abs.y < existingUserButton.second.second;
			if (clickedOnExistingUser) {
				JSON json;
				json["type"] = "'salts request'";
				json["data"] = JSON();
				auto username = player->getUserNamePw().first;
				json["data"]["username"] = "'" + username + "'";
				socket->send(json);
				return;
			}
		}
	}));

	MouseRightClickEvent::subscribe(new EventObserver<MouseRightClickEvent>([&](MouseRightClickEvent* ev) {
		if (gameData->userIsLoggedIn()) {
			if (!rightClickInterface->active || !rightClickInterface->mouseIsInRect(ev)) {
				rightClickInterface->setPosition(ev->pos);
				VTile tileClicked = converter.getPositionInGame(ev->pos);
				Tile* t = map->getTileFromVTile(tileClicked);
				if (t)
					t->onRightClick(*ev);
			}
		}
		else {

		}
	}));

	MouseMiddleClickEvent::subscribe(new EventObserver<MouseMiddleClickEvent>([&](MouseMiddleClickEvent* ev) {
		if (gameData->userIsLoggedIn()) {
			auto clickedOnRightClickInterface = rightClickInterface->active && rightClickInterface->mouseIsInRect(ev);
			if (clickedOnRightClickInterface)
				return;
			auto clickedOnRightBanner = rightBanner->mouseIsInRect(ev);
			if (clickedOnRightBanner)
				return;
			VTile tileClicked = converter.getPositionInGame(ev->pos);
			Tile* t = map->getTileFromVTile(tileClicked);
			if (t)
				t->onMiddleClick(*ev);
		}
		else {

		}
	}));

	MouseMoveEvent::subscribe(new EventObserver<MouseMoveEvent>([&](MouseMoveEvent* ev) {
		if (gameData->userIsLoggedIn()) {
			// add top left indicator of what your mouse is over + left click option + options.length
			if (rightClickInterface->active && !rightClickInterface->mouseIsInRect(ev)) {
				rightClickInterface->active = false;
			}
		}
		else {

		}
	}));

	ResizeEvent::subscribe(new EventObserver<ResizeEvent>([&](ResizeEvent* ev) {
		updateWindowSize();
	}));
}

void SFRenderWindow::draw(sf::VertexArray v, sf::RenderStates s) {
	window.draw(v, s);
}

void SFRenderWindow::draw(const sf::Shape* s, sf::Transform t) {
	window.draw(*s, t);
}

void SFRenderWindow::draw(const sf::Sprite s, sf::Transform t) {
	window.draw(s, t);
}

void SFRenderWindow::draw(const sf::Text text, sf::Transform t) {
	window.draw(text, t);
}

void SFRenderWindow::draw(VTile pos, double angle, sf::Sprite s) {
	const auto zoomScale = measures->zoom;
	const auto scale = measures->stretch;
	const auto cameraPos = camera->getPosition();
	VTile tileDelta(pos.x - cameraPos.x, pos.y - cameraPos.y);
	VPixel delta(tileDelta * AbstractMeasures::pixelsPerTile);
	const auto transform = sf::RenderStates().transform
		.rotate(angle)
		.scale(1 / scale.x, 1 / scale.y)
		.translate(
			measures->getInnerWindowSizeTile().x * AbstractMeasures::pixelsPerTile / 2,
			measures->getInnerWindowSizeTile().y * AbstractMeasures::pixelsPerTile / 2
		)
		.scale(zoomScale, zoomScale)
		.rotate(measures->angle)
		.translate(delta.x, delta.y)
		.rotate(-measures->angle)
		.translate(-(17.f / 16) * measures->getTileSize().x, -1.5 * measures->getTileSize().y);
	draw(s, transform);
}

VPixel SFRenderWindow::getSize() {
	VPixel v;
	auto size = window.getSize();
	v.x = size.x;
	v.y = size.y;
	return v;
}

void SFRenderWindow::setFramerateLimit(int limit) {
	window.setFramerateLimit(limit);
}

bool SFRenderWindow::isOpen() {
	return window.isOpen();
}

void SFRenderWindow::close() {
	if (window.isOpen())
		window.close();
}

void SFRenderWindow::clear() {
	window.clear();
}

void SFRenderWindow::display() {
	window.display();
	FrameEvent().emit();
}

void SFRenderWindow::events() {
	sf::Event event;
	while (window.pollEvent(event))
		if (event.type == sf::Event::Closed)
			window.close();
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

void SFRenderWindow::draw() {
	clear();
	if (gameData->userIsLoggedIn()) {
		update();
		auto playerPositions = gameData->getPlayerPositions();
		const VTile pos = camera->getPosition();
		VTile relativePos(
			pos.x - map->centerChunk.x * AbstractMeasures::TilesPerChunk - measures->getInnerWindowSizeTile().x / 2,
			pos.y - map->centerChunk.y * AbstractMeasures::TilesPerChunk - measures->getInnerWindowSizeTile().y / 2
		);
		auto getTransform = [&](const VTile& relativePos, const VChunk& chunkOffset) {
			AbstractMeasures* measures = (AbstractMeasures*)provider->get("Measures");
			const float scale = measures->zoom;
			VTile offsetTiles = VTile(chunkOffset.x * AbstractMeasures::TilesPerChunk, chunkOffset.y * AbstractMeasures::TilesPerChunk) - relativePos;
			const auto offset = VPixel(AbstractMeasures::pixelsPerTile * offsetTiles.x * scale, AbstractMeasures::pixelsPerTile * offsetTiles.y * scale);
			const VTile scalingDiff = measures->getInnerWindowSizeTile() * VTile(1 - scale, 1 - scale);
			const VPixel scalingDiffPx = VPixel(AbstractMeasures::pixelsPerTile * scalingDiff.x, AbstractMeasures::pixelsPerTile * scalingDiff.y) / 2;
			const auto finalOffset = offset + scalingDiffPx;

			sf::Transform transform;
			const sf::Vector2f middleOfInnerWindow(
				measures->getInnerWindowSizeTile().x * AbstractMeasures::pixelsPerTile / 2,
				measures->getInnerWindowSizeTile().y * AbstractMeasures::pixelsPerTile / 2
			);
			transform.scale(sf::Vector2f(1 / measures->stretch.x, 1 / measures->stretch.y));
			transform.rotate(measures->angle, middleOfInnerWindow);
			transform.translate(sf::Vector2f(finalOffset.x, finalOffset.y));
			transform.scale(scale, scale);
			return transform;
		};
		map->mutex.lock();
		for (int i = 0; i < 2 * map->chunkRadius + 1; ++i)
			for (int j = 0; j < 2 * map->chunkRadius + 1; ++j) {
				auto* chunk = map->loaded[i][j];
				if (chunk->deleted)
					return;
				sf::Transform transform = getTransform(relativePos + VTile(0.5, 0.5), VChunk(i, j) - VChunk(map->chunkRadius, map->chunkRadius));
				chunk->tilemap.draw(*this, transform);
				chunk->wallmap.draw(*this, transform);
				chunk->objectmap.draw(*this, transform);
			}
		map->mutex.unlock();

		for (int i = 0; i < playerPositions.size(); ++i)
			draw(playerPositions[i].second, 0, playerSprite);

		bottomBanner->draw();
		rightBanner->draw();
		rightClickInterface->draw();
	}
	else {
		window.draw(loginPage);

		sf::Font font;
		font.loadFromFile("../../../assets/runescape_uf.ttf");
		auto credentials = player->getUserNamePw();
		sf::Text username(credentials.first + "\n" + credentials.second, font);
		const auto scale = measures->stretch;
		sf::Transform transform;
		transform.translate(500, 400);
		transform.scale(sf::Vector2f(1 / scale.x, 1 / scale.y));
		draw(username, transform);
	}
	display();
}

void SFRenderWindow::update() {
	rightBanner->update();
}

void SFRenderWindow::setActive(bool newState = true) {
	window.setActive(newState);
}

void SFRenderWindow::updateWindowSize() {
	auto size = getSize();
	measures->windowSize = size;
	measures->stretch = VPixel(size.x / AbstractMeasures::startingScreenSize().x, size.y / AbstractMeasures::startingScreenSize().y);
}
