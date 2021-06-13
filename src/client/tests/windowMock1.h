#pragma once
#include "abstractRenderWindow.h"

class MockWindow : public AbstractRenderWindow {
private:
	bool open = true;
public:
	virtual VPixel getSize() { return VPixel(); };
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
	virtual void draw() {};
	virtual void update() {};
	virtual void init() {};
	virtual void setActive(bool newState) {};
};
