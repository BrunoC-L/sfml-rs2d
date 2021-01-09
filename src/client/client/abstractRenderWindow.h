#pragma once
#include "../../common/abstractService.h"
#include "mouseWheelEvent.h"
#include "../../common/units.h"

class AbstractRenderWindow : public AbstractService {
public:
	virtual VPixel getSize() = 0;
	virtual void setFramerateLimit(int limit) = 0;
	virtual bool isOpen() = 0;
	virtual void close() = 0;
	virtual void clear() = 0;
	virtual void display() = 0;
	virtual void draw(sf::VertexArray v, sf::RenderStates s) = 0;
	virtual void draw(const sf::Shape* s, sf::Transform t) = 0;
	virtual void draw(const sf::Sprite s, sf::Transform t) = 0;
	virtual void draw(const sf::Text text, sf::Transform t) = 0;
	virtual void events() = 0;
	virtual void draw(VTile pos, double angle, sf::Sprite s) = 0;
	virtual void draw() = 0;
	virtual void update() = 0;
};
