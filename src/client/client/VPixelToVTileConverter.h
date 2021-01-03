#pragma once
#include "units.h"
#include "abstractMeasures.h"
#include "SFML/Graphics.hpp"
#include "abstractCamera.h"
#include "abstractServices.h"
#include "service.h"

class VPixelToVTileConverter : public Service {
public:
	VPixelToVTileConverter(AbstractServiceProvider* provider) : Service(provider) {
		acquire();
	}
	 VTile getPositionInGame(VPixel mousePos) {
		VPixel middle(measures->getInnerWindowSize() / 2);
		VPixel delta = mousePos - middle;
		delta = VPixel(delta.x ? delta.x : 1, delta.y ? delta.y : 1);
		const float radius = pow(pow(delta.x, 2) + pow(delta.y, 2), 0.5f);
		const float angle = (delta.x == 0 ? (delta.y > 0 ? 90 : -90) : (delta.x > 0 ? 0 : 3.1415926536f) + atan(delta.y / delta.x)) - measures->angle / 180 * 3.1415926536f;
		VPixel rotatedDelta = VPixel(cos(angle), sin(angle)) * radius;
		VTile signs(rotatedDelta.x > 0 ? 1 : -1, rotatedDelta.y > 0 ? 1 : -1);
		rotatedDelta *= VPixel(signs.x, signs.y);
		rotatedDelta /= measures->zoom;
		VTile deltaTilesFloat = VTile(rotatedDelta.x, rotatedDelta.y) / AbstractMeasures::pixelsPerTile;
		VTile tileClicked = camera->getPosition() + VTile(deltaTilesFloat.x * signs.x, deltaTilesFloat.y * signs.y) + VTile(0.5, 0.5);
		return tileClicked;
	}
};