#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "resizeEvent.h"

class AbstractMeasures;

class AnchorTransform {
private:
	sf::Transform transform;
	AbstractMeasures* measures;
	VPixel anchor;
	EventObserver<ResizeEvent> resizeObserver;
	void update();
public:
	AnchorTransform(VPixel anchor);
	const sf::Transform& getTransform() const;
	void setMeasures(AbstractMeasures* m);
	VPixel getPosition() const;
};

extern AnchorTransform BottomRightAnchorTransform;
extern AnchorTransform BottomLeftAnchorTransform;
extern AnchorTransform TopRightAnchorTransform;
extern AnchorTransform TopLeftAnchorTransform;
