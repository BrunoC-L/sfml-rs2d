#pragma once
#include "hoverableComponent.h"
#include "clickableComponent.h"
#include "anchoredOffsetTransform.h"
#include "abstractRenderWindow.h"
#include <memory>

class Button {
protected:
	std::shared_ptr<HoverableComponent> hover;
	std::shared_ptr<ClickableComponent> click;
	const std::shared_ptr<AnchorTransform> aot;
	sf::RectangleShape shape;
	VPixel size;
	std::string strtext;
	sf::Text sftext;
public:
	Button(
		std::shared_ptr<AnchorTransform>&& aot,
		VPixel size,
		const sf::Texture& texture
	);

	Button(
		std::shared_ptr<AnchorTransform>&& aot,
		VPixel size,
		sf::Color color
	);

	Button(
		std::shared_ptr<AnchorTransform>&& aot,
		VPixel size
	);

	void onClick(std::function<void()> onClick);
	void onOffHover(std::function<void()> onHover, std::function<void()> offHover);

	void text(std::string&& text, sf::Color color);
	void texture(const sf::Texture& texture);

	void draw(AbstractRenderWindow& window);
};
