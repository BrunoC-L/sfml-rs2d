#pragma once
#include "anchorTransform.h"

class AnchoredOffsetTransform {
private:
	const AnchorTransform& anchorTransform;
	const VPixel offset;
public:
	AnchoredOffsetTransform(const AnchorTransform& anchorTransform, VPixel offset);
	sf::Transform&& getTransform();
};
