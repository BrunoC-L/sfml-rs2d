#include "sfRenderWindow.h"
#include "keyPressedEvent.h"
#include "resizeEvent.h"

SFRenderWindow::SFRenderWindow(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("RenderWindow", this);
}

void SFRenderWindow::init() {
	acquire();
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
	renderWindow->draw(s, transform);
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