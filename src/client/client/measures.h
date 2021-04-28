#pragma once
#include "units.h"
#include "keyPressedEvent.h"
#include "service.h"

class Measures : public AbstractMeasures, public Service {
	EventObserver<LeftArrowKeyPressedEvent> leftObserver;
	EventObserver<RightArrowKeyPressedEvent> rightObserver;
	EventObserver<MouseWheelEvent> mouseWheelObserver;
public:
	Measures(ServiceProvider* provider) : Service(provider) {
		provider->set("Measures", this);
	}
	void init();

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
