#pragma once
#include "abstractRenderWindow.h"
#include "frameEvent.h"
#include "abstractMeasures.h"

class MockWindow : public AbstractRenderWindow {
private:
	bool open = false;
public:
	virtual VPixel getSize() { return AbstractMeasures::startingScreenSize(); };
	virtual void setFramerateLimit(int limit) {};
	virtual bool isOpen() { return open; };
	virtual void close() { open = false; };
	virtual void clear() {};
	virtual void display() {};
	virtual void events() {};
	virtual void draw(const sf::VertexArray& v, const sf::RenderStates& s) {};
	virtual void draw(const sf::Shape* s, const sf::Transform& t) {};
	virtual void draw(const sf::Shape& s, sf::Transform t) {};
	virtual void draw(const sf::Sprite& s, const sf::Transform& t) {};
	virtual void draw(const sf::Text& text, const sf::Transform& t) {};
	virtual void draw(VTile pos, double angle, const sf::Sprite& s) {};
	virtual void draw(TileMap* tilemap, const sf::Transform& t) {};
	virtual void draw() {
		FrameEvent().emit();
	};
	virtual void update() {};
	virtual void init() {
		open = true;
	};
	virtual void setActive(bool newState) {};
};
