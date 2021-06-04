#pragma once
#include "component.h"
#include <functional>
#include "mouseMoveEvent.h"

class HoverableComponent : public Component {
protected:
	std::function<void()> onHover;
	std::function<void()> offHover;
	EventObserver<MouseMoveEvent> moveObserver;
	bool hovered;
public:
	HoverableComponent(std::function<void()> onHover, std::function<void()> offHover, const AnchoredOffsetTransform& aot, VPixel size);
};
