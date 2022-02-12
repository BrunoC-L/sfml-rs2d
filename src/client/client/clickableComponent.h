#pragma once
#include "component.h"
#include <functional>
#include "mouseLeftClickEvent.h"

class ClickableComponent : public Component {
protected:
	std::function<void()> onClick;
	MouseLeftClickEvent::Observer clickObserver;
public:
	ClickableComponent(std::function<void()> onClick, const AnchorTransform& aot, VPixel size);
};
