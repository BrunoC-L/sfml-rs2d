#pragma once
#include "../../common/common/units.h"
#include "../../common/common/abstractService.h"

using namespace std;

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
		return VPixel(startingInnerScreenSizeTile() * pixelsPerTile);
	}

	static VPixel banners() {
		return VPixel(rightBannerWidth, bottomBannerHeight);
	}

	static VPixel startingScreenSize() {
		return VPixel(startingInnerScreenSizeTile().x * pixelsPerTile,
			startingInnerScreenSizeTile().y * pixelsPerTile) + banners();
	}

	VPixel stretch = VPixel(1, 1);
	VPixel windowSize;
	float angle = 0;
	float zoom = 1;
	static constexpr float rightBannerWidth = 230;
	static constexpr float bottomBannerHeight = 180;
	static constexpr float TilesPerChunk = 64;
	static constexpr float pixelsPerTileOnMap = 4;
	static constexpr float minimapRadius = 100;
	static constexpr float pixelsPerTile = 32;
	static constexpr float framesPerTick = 60;
	static constexpr float itemsSizePx = 128;
	static constexpr float itemsSizeInInventory = 55;
};
