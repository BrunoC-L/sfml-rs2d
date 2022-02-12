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
#include "login.h"

class SFRenderWindow : public AbstractRenderWindow, public Service {
	MouseLeftClickEvent::Observer leftClickObserver;
	MouseRightClickEvent::Observer rightClickObserver;
	MouseMiddleClickEvent::Observer middleClickObserver;
	MouseMoveEvent::Observer mouseMoveObserver;
	ResizeEvent::Observer resizeObserver;
	VPixelToVTileConverter converter;
	std::shared_ptr<RightBanner> rightBanner;
	std::shared_ptr<BottomBanner> bottomBanner;
	std::shared_ptr<RightClickInterface> rightClickInterface;
	sf::RenderWindow& window;
	sf::RectangleShape loginPage;
	sf::Texture loginTexture = sf::Texture();
	sf::Texture p_t;
	sf::Sprite playerSprite;
	void updateWindowSize();

	std::shared_ptr<Button> signUpButton;
	std::shared_ptr<Button> loginButton;

	LoginEvent::Observer  loginObserver;
	LogoutEvent::Observer logoutObserver;
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
