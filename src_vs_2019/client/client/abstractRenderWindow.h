#pragma once
#include <SFML/Graphics.hpp>
#include "abstractService.h"
#include "mouseWheelEvent.h"
#include "units.h"

class TileMap;

class AbstractRenderWindow {
public:
	virtual VPixel getSize() = 0;
	virtual void setFramerateLimit(int limit) = 0;
	virtual bool isOpen() = 0;
	virtual void close() = 0;
	virtual void clear() = 0;
	virtual void display() = 0;
	virtual void events() = 0;
	virtual void draw(const sf::VertexArray& v, const sf::RenderStates& s) = 0;
	virtual void draw(const sf::Shape* s, const sf::Transform& t) = 0;
	virtual void draw(const sf::Shape& s, sf::Transform t) = 0;
	virtual void draw(const sf::Sprite& s, const sf::Transform& t) = 0;
	virtual void draw(const sf::Text& text, const sf::Transform& t) = 0;
	virtual void draw(VTile pos, double angle, const sf::Sprite& s) = 0;
	virtual void draw(TileMap* tilemap, const sf::Transform& t) = 0;
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void init() = 0;
	virtual void setActive(bool newState) = 0;
};
