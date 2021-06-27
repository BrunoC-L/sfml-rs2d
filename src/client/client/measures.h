#pragma once
#include "units.h"
#include "keyPressedEvent.h"
#include "service.h"
#include "chunk.h"

class Measures : public AbstractMeasures, public Service {
	EventObserver<LeftArrowKeyPressedEvent> leftObserver;
	EventObserver<RightArrowKeyPressedEvent> rightObserver;
	EventObserver<MouseWheelEvent> mouseWheelObserver;
public:
	Measures(ServiceProvider* provider) : Service(provider) {
		provider->set(MEASURES, this);
	}
	void init();

	virtual VTile getInnerWindowSizeTile() const {
		VPixel size = windowSize;
		VPixel banners(rightBannerWidth, bottomBannerHeight);
		size -= banners;
		return VTile(size.x, size.y) / PIXELS_PER_TILE;
	}

	virtual VPixel getInnerWindowSize() const {
		VPixel size = windowSize;
		VPixel banners(rightBannerWidth, bottomBannerHeight);
		return size - banners;
	}

	virtual VPixel getTileSize() const {
		return VPixel(PIXELS_PER_TILE, PIXELS_PER_TILE);
	}

	virtual float getRightBannerStartingX()  const {
		return AbstractMeasures::startingScreenSize().x - AbstractMeasures::banners().x;
	}

	virtual float getBottomBannerStartingY() const {
		return AbstractMeasures::startingScreenSize().y - AbstractMeasures::banners().y;
	}
};
