#include "anchoredOffsetTransform.h"

AnchoredOffsetTransform::AnchoredOffsetTransform(const AnchorTransform& anchorTransform, VPixel offset) : anchorTransform(anchorTransform), offset(offset) {

}

sf::Transform&& AnchoredOffsetTransform::getTransform() {
	sf::Transform t(anchorTransform.getTransform());
	t.translate(offset.x, offset.y);
	return std::move(t);
}
