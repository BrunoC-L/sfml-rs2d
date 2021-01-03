#pragma once
#include "camera.h"

Camera::Camera(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("Camera", this);
}
void Camera::init() {
	acquire();
	setPosition(&(player->position));
}
void Camera::setPosition(VTile* pos) {
	position = pos;
}
VTile& Camera::getPosition() {
	return *position;
}
