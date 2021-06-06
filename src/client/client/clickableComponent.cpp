#include "clickableComponent.h"

ClickableComponent::ClickableComponent(std::function<void()> onClick, const AnchorTransform& aot, VPixel size) : onClick(onClick), Component(aot, size) {
	clickObserver.set([&](MouseLeftClickEvent& ev) {
		auto rect = getRect();
		if (ev.pos.x >= rect.first.x && ev.pos.x <= rect.second.x && ev.pos.y >= rect.first.y && ev.pos.y <= rect.second.y)
			this->onClick();
		});
}
