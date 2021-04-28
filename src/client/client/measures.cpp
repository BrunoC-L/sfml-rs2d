#include "measures.h"

void Measures::init() {
	acquire();

	leftObserver.set([&](LeftArrowKeyPressedEvent& ev) {
		angle -= 5;
	});
	rightObserver.set([&](RightArrowKeyPressedEvent& ev) {
		angle += 5;
	});
	mouseWheelObserver.set([&](MouseWheelEvent& ev) {
		zoom = zoom * (1 + (0.3 + ev.delta) * 0.1f);
	});
}
