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
	const AnchoredOffsetTransform aot;
	sf::RectangleShape shape;
	VPixel size;
public:
	Button(
		AnchoredOffsetTransform&& aot,
		VPixel size,
		const sf::Texture& texture
	);

	Button(
		AnchoredOffsetTransform&& aot,
		VPixel size,
		sf::Color color
	);

	void onClick(std::function<void()> onClick);
	void onOffHover(std::function<void()> onHover, std::function<void()> offHover);

	void draw(AbstractRenderWindow& window);
};
