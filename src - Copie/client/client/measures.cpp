#include "measures.h"

void Measures::init() {
	acquire();

	leftObserver.set([&](const LeftArrowKeyPressedEvent::Data& ev) {
		angle -= 5;
	});
	rightObserver.set([&](const RightArrowKeyPressedEvent::Data& ev) {
		angle += 5;
	});
	mouseWheelObserver.set([&](const MouseWheelEvent::Data& ev) {
		zoom = zoom * (1 + (0.3 + ev.delta) * 0.1f);
	});
}
