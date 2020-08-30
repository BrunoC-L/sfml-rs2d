#pragma once
#include "units.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

using namespace sf;
using namespace std;

class Measures {
	Measures() {
		startingInnerScreenSizeTile = VTile(35, 23);
		auto inpx = startingInnerScreenSizeTile * pixelsPerTile;
		startingInnerScreenSize = VPixel(inpx.x, inpx.y);
		banners = VPixel(rightBannerWidth, bottomBannerHeight);
		startingScreenSize = VPixel(startingInnerScreenSizeTile.x * pixelsPerTile, startingInnerScreenSizeTile.y * pixelsPerTile) + banners;
	}
public:
	Measures(const Measures& other) = delete;
	Measures operator=(const Measures& other) = delete;
	static Measures& getInstance() {
		static Measures instance;
		return instance;
	}
	void setGetWindowSize(std::function<VPixel()> f) { getWindowSize = f; };
	void update() { stretch = Vector2f(getWindowSize().x / startingScreenSize.x, getWindowSize().y / startingScreenSize.y); }

	VTile getInnerWindowSizeTile() const { VPixel size = getWindowSize(); VPixel banners(rightBannerWidth, bottomBannerHeight); size -= banners; return VTile(size.x, size.y) / 32; }
	VPixel getInnerWindowSize() const { VPixel size = getWindowSize(); VPixel banners(rightBannerWidth, bottomBannerHeight); return size - banners; }
	
	Vector2f getTileSize()         const { return Vector2f(pixelsPerTile, pixelsPerTile); }

	float getRightBannerStartingX()   const { return startingScreenSize.x - banners.x; }
	float getBottomBannerStartingY()  const { return startingScreenSize.y - banners.y; }

	VTile  startingInnerScreenSizeTile;
	VPixel startingInnerScreenSize;
	VPixel banners;
	VPixel startingScreenSize;
	std::function<VPixel()> getWindowSize;
	Vector2f stretch = Vector2f(1, 1);
	float angle = 0;
	float zoom = 1;
	static constexpr float rightBannerWidth = 230;
	static constexpr float bottomBannerHeight = 180;
	static constexpr float TilesPerChunk = 64;
	static constexpr float pixelsPerTileOnMap = 4;
	static constexpr float minimapRadius = 100;
	static constexpr float pixelsPerTile = 32;
	static constexpr float framesPerTick = 6;
	static constexpr float itemsSizePx = 128;
	static constexpr float itemsSizeInInventory = 55;
};
