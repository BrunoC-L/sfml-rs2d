#include "anchoredOffsetTransform.h"

AnchoredOffsetTransform::AnchoredOffsetTransform(const AnchorTransform& anchorTransform, VPixel offset) : anchorTransform(anchorTransform), offset(offset), AnchorTransform(VPixel()) {

}

const sf::Transform& AnchoredOffsetTransform::getTransform() const {
	sf::Transform t(anchorTransform.getTransform());
	t.translate(offset.x, offset.y);
	return t;
}

VPixel AnchoredOffsetTransform::getPosition() const {
	return anchorTransform.getPosition() + offset;
}
