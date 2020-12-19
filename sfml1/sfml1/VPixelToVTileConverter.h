#pragma once
#include "units.h"
#include "measures.h"
#include "SFML/Graphics.hpp"
#include "camera.h"

class VPixelToVTileConverter {
public:
	static VPixelToVTileConverter& getInstance() {
		static VPixelToVTileConverter instance;
		return instance;
	}
	VTile getPositionInGame(VPixel mousePos) {
		Measures& measures = Measures::getInstance();
		VPixel middle(measures.getInnerWindowSize() / 2);
		VPixel delta = mousePos - middle;
		delta = VPixel(delta.x ? delta.x : 1, delta.y ? delta.y : 1);
		const float radius = pow(pow(delta.x, 2) + pow(delta.y, 2), 0.5f);
		const float angle = (delta.x == 0 ? (delta.y > 0 ? 90 : -90) : (delta.x > 0 ? 0 : 3.1415926536f) + atan(delta.y / delta.x)) - measures.angle / 180 * 3.1415926536f;
		VPixel rotatedDelta = VPixel(cos(angle), sin(angle)) * radius;
		VTile signs(rotatedDelta.x > 0 ? 1 : -1, rotatedDelta.y > 0 ? 1 : -1);
		rotatedDelta *= VPixel(signs.x, signs.y);
		rotatedDelta /= measures.zoom;
		VTile deltaTilesFloat = VTile(rotatedDelta.x, rotatedDelta.y) / Measures::pixelsPerTile;
		VTile tileClicked = Camera::getInstance().getPosition() + VTile(deltaTilesFloat.x * signs.x, deltaTilesFloat.y * signs.y) + VTile(0.5, 0.5);
		return tileClicked;
	}
private:
	VPixelToVTileConverter& operator=(VPixelToVTileConverter& other) = delete;
	VPixelToVTileConverter(VPixelToVTileConverter& other) = delete;
	VPixelToVTileConverter() = default;
};