#pragma once
#include "../client/abstractRenderWindow.h"

class MockRenderWindow : public AbstractRenderWindow, public Service {
public:
	MockRenderWindow(AbstractServiceProvider* provider) : Service(provider) { }
	bool opened = true;
	void events() { }
	void draw() { }
	VPixel getSize() { return VTile(); }
	void setFramerateLimit(int) { }
	bool isOpen() { return opened; }
	void close() { opened = false; }
	virtual void clear() { }
	virtual void display() { }
	virtual void draw(sf::VertexArray v, sf::RenderStates s) { }
	virtual void draw(const sf::Shape* s, sf::Transform t) { }
	virtual void draw(const sf::Sprite s, sf::Transform t) { }
	virtual void draw(const sf::Text text, sf::Transform t) { }
	virtual void draw(VTile pos, double angle, sf::Sprite s) { }
	virtual void update() { }
	virtual void init() { }
};
