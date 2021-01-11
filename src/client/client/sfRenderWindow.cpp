#include "sfRenderWindow.h"
#include "keyPressedEvent.h"
#include "resizeEvent.h"

SFRenderWindow::SFRenderWindow(AbstractServiceProvider* provider) : Service(provider), converter(provider) { }

void SFRenderWindow::init() {
	acquire();

	setFramerateLimit(60);

	rightBanner = new RightBanner(provider, this);
	bottomBanner = new BottomBanner(provider, this);
	rightClickInterface = new RightClickInterface(provider, this);

	MouseLeftClickEvent::subscribe(new EventObserver<MouseLeftClickEvent>([&](MouseLeftClickEvent* ev) {
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
		VTile tileClicked = converter.getPositionInGame(ev->pos);
		Tile* t = map->getTileFromVTile(tileClicked);
		if (t)
			t->onLeftClick(*ev);
	}));

	MouseRightClickEvent::subscribe(new EventObserver<MouseRightClickEvent>([&](MouseRightClickEvent* ev) {
		if (!rightClickInterface->active || !rightClickInterface->mouseIsInRect(ev)) {
			rightClickInterface->setPosition(ev->pos);
			VTile tileClicked = converter.getPositionInGame(ev->pos);
			Tile* t = map->getTileFromVTile(tileClicked);
			if (t)
				t->onRightClick(*ev);
		}
	}));

	MouseMiddleClickEvent::subscribe(new EventObserver<MouseMiddleClickEvent>([&](MouseMiddleClickEvent* ev) {
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
	}));

	MouseMoveEvent::subscribe(new EventObserver<MouseMoveEvent>([&](MouseMoveEvent* ev) {
		// add top left indicator of what your mouse is over + left click option + options.length
		if (rightClickInterface->active && !rightClickInterface->mouseIsInRect(ev)) {
			rightClickInterface->active = false;
		}
	}));

	ResizeEvent::subscribe(new EventObserver<ResizeEvent>([&](ResizeEvent* ev) {
		auto size = getSize();
		measures->windowSize = size;
		measures->stretch = sf::Vector2f(size.x / AbstractMeasures::startingScreenSize().x, size.y / AbstractMeasures::startingScreenSize().y);
	}));
	auto size = getSize();
	measures->windowSize = size;
	measures->stretch = sf::Vector2f(size.x / AbstractMeasures::startingScreenSize().x, size.y / AbstractMeasures::startingScreenSize().y);
}

void SFRenderWindow::draw(sf::VertexArray v, sf::RenderStates s) {
	getInstance().draw(v, s);
}

void SFRenderWindow::draw(const sf::Shape* s, sf::Transform t) {
	getInstance().draw(*s, t);
}

void SFRenderWindow::draw(const sf::Sprite s, sf::Transform t) {
	getInstance().draw(s, t);
}

void SFRenderWindow::draw(const sf::Text text, sf::Transform t) {
	getInstance().draw(text, t);
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
	auto size = getInstance().getSize();
	v.x = size.x;
	v.y = size.y;
	return v;
}

void SFRenderWindow::setFramerateLimit(int limit) {
	getInstance().setFramerateLimit(limit);
}

bool SFRenderWindow::isOpen() {
	auto isOpen = getInstance().isOpen();
	auto wtf = getInstance().getSize();
	return isOpen;
}

void SFRenderWindow::close() {
	getInstance().close();
}

void SFRenderWindow::clear() {
	getInstance().clear();
}

void SFRenderWindow::display() {
	getInstance().display();
}

void SFRenderWindow::events() {
	sf::Event event;
	while (getInstance().pollEvent(event))
		if (event.type == sf::Event::Closed)
			getInstance().close();
		else if (event.type == sf::Event::KeyPressed)
			switch (event.text.unicode) {
			case 71:
				LeftArrowKeyPressedEvent().emit();
				break;
			case 72:
				RightArrowKeyPressedEvent().emit();
				break;
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
	VTile pos = *camera->position;
	cout << pos.x << ' ' << pos.y << '\n';
	VTile relativePos(
		pos.x - map->centerChunk.x * AbstractMeasures::TilesPerChunk - measures->getInnerWindowSizeTile().x / 2,
		pos.y - map->centerChunk.y * AbstractMeasures::TilesPerChunk - measures->getInnerWindowSizeTile().y / 2
	);
	map->mutex.lock();
	for (int i = 0; i < 2 * map->chunkRadius + 1; ++i)
		for (int j = 0; j < 2 * map->chunkRadius + 1; ++j)
			map->loaded[i][j]->draw(*this, relativePos + VTile(0.5, 0.5), VChunk(i, j) - VChunk(map->chunkRadius, map->chunkRadius));
	map->mutex.unlock();
	draw(player->position, 0, player->playerSprite);

	for (int i = 0; i < gameData->playerPositions.size(); ++i) {
		if (i == player->id)
			continue;
		auto pos = gameData->playerPositions[i];
		draw(pos, 0, player->playerSprite);
	}

	bottomBanner->draw();
	rightBanner->draw();
	rightClickInterface->draw();
}

void SFRenderWindow::update() {
	rightBanner->update();
}
