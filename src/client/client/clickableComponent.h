#pragma once
#include "component.h"
#include <functional>
#include "mouseLeftClickEvent.h"

class ClickableComponent : public Component {
protected:
	std::function<void()> onClick;
	EventObserver<MouseLeftClickEvent> clickObserver;
public:
	ClickableComponent(std::function<void()> onClick, const AnchoredOffsetTransform& aot, VPixel size);
};
