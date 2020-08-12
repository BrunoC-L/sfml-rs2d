#pragma once
#include "units.h"
#include "player.h"

class Camera {
private:
	Camera() = default;
	static VTile* position;
public:
	void setPosition(VTile* pos) {
		position = pos;
	}
	static VTile& getPosition() {
		return *position;
	}
	Camera(const Camera& other) = delete;
	Camera operator=(const Camera& other) = delete;
	static Camera& getInstance() {
		static Camera instance;
		return instance;
	}
};
