#pragma once
#include "units.h"
#include "abstractService.h"
#include "constants.h"

enum MouseClickType { Left, Right, Middle };

class AbstractMeasures : public AbstractService {
public:
	virtual VTile getInnerWindowSizeTile() const = 0;
	virtual VPixel getInnerWindowSize() const = 0;
	virtual VPixel getTileSize() const = 0;
	virtual float getRightBannerStartingX() const = 0;
	virtual float getBottomBannerStartingY() const = 0;

	static VTile startingInnerScreenSizeTile() {
		return VTile(35, 23);
	}

	static VPixel startingInnerScreenSize() {
		return VPixel(startingInnerScreenSizeTile() * PIXELS_PER_TILE);
	}

	static VPixel banners() {
		return VPixel(rightBannerWidth, bottomBannerHeight);
	}

	static VPixel startingScreenSize() {
		return VPixel(startingInnerScreenSizeTile().x * PIXELS_PER_TILE,
			startingInnerScreenSizeTile().y * PIXELS_PER_TILE) + banners();
	}

	VPixel stretch = VPixel(1, 1);
	VPixel windowSize;
	float angle = 0;
	float zoom = 1;
	static constexpr float rightBannerWidth = 245;
	static constexpr float bottomBannerHeight = 180;
	static constexpr float minimapRadius = 100;
	static constexpr float framesPerTick = 60;
};
