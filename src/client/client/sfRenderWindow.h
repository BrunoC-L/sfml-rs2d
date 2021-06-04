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
#include "chunk.h"
#include "anchoredOffsetTransform.h"
#include "button.h"

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
	sf::Font loginfont;
	void updateWindowSize();


	sf::RectangleShape red;
	AnchoredOffsetTransform redt;
	sf::RectangleShape blue;
	AnchoredOffsetTransform bluet;
	sf::RectangleShape green;
	AnchoredOffsetTransform greent;
	//sf::RectangleShape yellow;
	//AnchoredOffsetTransform yellowt;
	sf::RectangleShape pink;
	std::shared_ptr<Button> yellowButton;



public:
	SFRenderWindow(ServiceProvider* provider, sf::RenderWindow& window);
	void init();
	virtual void draw(const sf::VertexArray& v, const sf::RenderStates& s) override;
	virtual void draw(const sf::Shape* s, const sf::Transform& t) override;
	virtual void draw(const sf::Shape& s, sf::Transform t) override;
	virtual void draw(const sf::Sprite& s, const sf::Transform& t) override;
	virtual void draw(const sf::Text& text, const sf::Transform& t) override;
	virtual void draw(VTile pos, double angle, const sf::Sprite& s) override;
	virtual void draw(TileMap* tilemap, const sf::Transform& t) override;
	virtual VPixel getSize() override;
	virtual void setFramerateLimit(int limit) override;
	virtual bool isOpen() override;
	virtual void close() override;
	virtual void clear() override;
	virtual void display() override;
	virtual void events() override;
	virtual void draw() override;
	virtual void update() override;
	virtual void setActive(bool newState);
};
