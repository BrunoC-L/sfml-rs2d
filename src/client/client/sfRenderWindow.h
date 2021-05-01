#pragma once
#include "SFML/Graphics.hpp"
#include "abstractServices.h"
#include "service.h"
#include "rightBanner.h"
#include "bottomBanner.h"
#include "rightClickInterface.h"
#include "VPixelToVTileConverter.h"
#include "keyPressedEvent.h"
#include "resizeEvent.h"
#include "frameEvent.h"

class SFRenderWindow : public AbstractRenderWindow, public Service {
	EventObserver<MouseLeftClickEvent> leftClickObserver;
	EventObserver<MouseRightClickEvent> rightClickObserver;
	EventObserver<MouseMiddleClickEvent> middleClickObserver;
	EventObserver<MouseMoveEvent> mouseMoveObserver;
	EventObserver<ResizeEvent> resizeObserver;
	VPixelToVTileConverter converter;
	std::shared_ptr<RightBanner> rightBanner;
	std::shared_ptr<BottomBanner> bottomBanner;
	std::shared_ptr<RightClickInterface> rightClickInterface;
	sf::RenderWindow& window;
	sf::RectangleShape loginPage;
	sf::Texture loginTexture = sf::Texture();
	sf::Texture p_t;
	sf::Sprite playerSprite;
public:
	SFRenderWindow(ServiceProvider* provider, sf::RenderWindow& window);
	void init();
	virtual void draw(sf::VertexArray v, sf::RenderStates s);
	virtual void draw(const sf::Shape* s, sf::Transform t);
	virtual void draw(const sf::Sprite s, sf::Transform t);
	virtual void draw(const sf::Text text, sf::Transform t);
	virtual void draw(VTile pos, double angle, sf::Sprite s);
	virtual VPixel getSize();
	virtual void setFramerateLimit(int limit);
	virtual bool isOpen();
	virtual void close();
	virtual void clear();
	virtual void display();
	virtual void events();
	virtual void draw();
	virtual void update();
	virtual void setActive(bool newState);
	void updateWindowSize();
};
