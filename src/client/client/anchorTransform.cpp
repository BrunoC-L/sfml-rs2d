#include "anchorTransform.h"
#include "abstractMeasures.h"

AnchorTransform::AnchorTransform(VPixel anchor) : measures(nullptr), anchor(anchor) {
}

void AnchorTransform::update() {
	sf::Transform t;
	t.scale(1 / measures->stretch.x, 1 / measures->stretch.y);
	t.translate(anchor.x, anchor.y);
	auto sizeChange = AbstractMeasures::startingScreenSize() * (measures->stretch - VPixel(1, 1));
	auto effectiveSizeChange = VPixel(
		sizeChange.x * anchor.x / AbstractMeasures::startingScreenSize().x,
		sizeChange.y * anchor.y / AbstractMeasures::startingScreenSize().y
	);
	t.translate(effectiveSizeChange.x, effectiveSizeChange.y);
	transform = t;
}

const sf::Transform& AnchorTransform::getTransform() const {
	return transform;
}

void AnchorTransform::setMeasures(AbstractMeasures* m) {
	this->measures = m;
	resizeObserver.set([&](ResizeEvent& ev) {
		update();
	});
	update();
}

AnchorTransform BottomRightAnchorTransform = AnchorTransform(VPixel(AbstractMeasures::startingScreenSize().x, AbstractMeasures::startingScreenSize().y));
AnchorTransform BottomLeftAnchorTransform  = AnchorTransform(VPixel(                                       0, AbstractMeasures::startingScreenSize().y));
AnchorTransform TopRightAnchorTransform    = AnchorTransform(VPixel(AbstractMeasures::startingScreenSize().x,                                        0));
AnchorTransform TopLeftAnchorTransform     = AnchorTransform(VPixel(                                       0,                                        0));
