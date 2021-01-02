#pragma once
#include "camera.h"

Camera::Camera(AbstractServiceProvider* provider) {
	REGISTER(Camera);
}
void Camera::init() {
	ACQUIRE;
	setPosition(&(player->position));
}
void Camera::setPosition(VTile* pos) {
	position = pos;
}
VTile& Camera::getPosition() {
	return *position;
}
