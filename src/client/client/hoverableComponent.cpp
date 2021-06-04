#include "hoverableComponent.h"

HoverableComponent::HoverableComponent(
	std::function<void()> onHover,
	std::function<void()> offHover,
	const AnchoredOffsetTransform& aot,
	VPixel size
) : onHover(onHover), offHover(offHover), Component(aot, size) {
	moveObserver.set([&](MouseEvent& ev) {
		auto rect = getRect();
		if (ev.pos.x >= rect.first.x && ev.pos.x <= rect.second.x && ev.pos.y >= rect.first.y && ev.pos.y <= rect.second.y) {
			if (!hovered)
				this->onHover();
		}
		else {
			if (hovered)
				this->offHover();
		}
		});
}
