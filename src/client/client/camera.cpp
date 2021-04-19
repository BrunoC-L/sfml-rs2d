#pragma once
#include "camera.h"

Camera::Camera(AbstractServiceProvider* provider) : Service(provider) {
	provider->set("Camera", this);
}

void Camera::init() {
	acquire();
	setPosition(&(player->getPosition()));
}

void Camera::setPosition(const VTile* pos) {
	position = pos;
}

const VTile& Camera::getPosition() {
	return *position;
}
