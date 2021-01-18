#pragma once
#include "../../common/common/units.h"
#include "keyPressedEvent.h"
#include "service.h"

using namespace std;

class Measures : public AbstractMeasures, public Service {
public:
	Measures(AbstractServiceProvider* provider) : Service(provider) {
		provider->set("Measures", this);
	}
	void init() {
		acquire();
		LeftArrowKeyPressedEvent::subscribe(
			new EventObserver<LeftArrowKeyPressedEvent>(
				[&](LeftArrowKeyPressedEvent* ev) {
					angle -= 5;
				}
			)
		);
		RightArrowKeyPressedEvent::subscribe(
			new EventObserver<RightArrowKeyPressedEvent>(
				[&](RightArrowKeyPressedEvent* ev) {
					angle += 5;
				}
			)
		);
		MouseWheelEvent::subscribe(
			new EventObserver<MouseWheelEvent>(
				[&](MouseWheelEvent* ev) {
					measures->zoom = measures->zoom * (1 + (0.3 + ev->delta) * 0.1f);
				}
			)
		);
	}

	virtual VTile getInnerWindowSizeTile() const {
		VPixel size = windowSize;
		VPixel banners(rightBannerWidth, bottomBannerHeight);
		size -= banners;
		return VTile(size.x, size.y) / 32;
	}

	virtual VPixel getInnerWindowSize() const {
		VPixel size = windowSize;
		VPixel banners(rightBannerWidth, bottomBannerHeight);
		return size - banners;
	}

	virtual VPixel getTileSize() const {
		return VPixel(pixelsPerTile, pixelsPerTile);
	}

	virtual float getRightBannerStartingX()  const {
		return AbstractMeasures::startingScreenSize().x - AbstractMeasures::banners().x;
	}

	virtual float getBottomBannerStartingY() const {
		return AbstractMeasures::startingScreenSize().y - AbstractMeasures::banners().y;
	}
};
