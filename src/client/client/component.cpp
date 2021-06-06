#include "component.h"

std::pair<VPixel, VPixel> Component::getRect() {
	auto pos = aot.getPosition();
	return { pos, pos + size };
}

Component::Component(const AnchorTransform& aot, VPixel size) : aot(aot), size(size) {
	
}
