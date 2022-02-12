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
	ResizeEvent::Observer resizeObserver;
	void update();
public:
	AnchorTransform(VPixel anchor);
	void setMeasures(AbstractMeasures* m);
	virtual const sf::Transform getTransform() const;
	virtual VPixel getPosition() const;
};

extern AnchorTransform BottomRightAnchorTransform;
extern AnchorTransform BottomLeftAnchorTransform;
extern AnchorTransform TopRightAnchorTransform;
extern AnchorTransform TopLeftAnchorTransform;
extern AnchorTransform MiddleAnchorTransform;
