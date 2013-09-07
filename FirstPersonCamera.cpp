/*
 * FirstPersonCamera.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: Tyler
 */

#include "FirstPersonCamera.h"

#include <glm/gtx/transform.hpp>

#include "MathHelper.h"

#include <iostream>

namespace ts {

FirstPersonCamera::FirstPersonCamera() {
	init(glm::vec3(0, 0, 0));
}

FirstPersonCamera::FirstPersonCamera(glm::vec3 position) {
	init(position);
}

void FirstPersonCamera::init(glm::vec3 position) {
	yaw = 90;
	pitch = 0;
	lookSpeed = 0.2f;
	this->position = position;
	lookAt = position + glm::vec3(0, 0, 1);
	flag_shouldUpdate = true;
}

FirstPersonCamera::~FirstPersonCamera() {
}

void FirstPersonCamera::setPosition(glm::vec3 newPosition) {
	glm::vec3 moveVector = newPosition - position;
	position += moveVector;
	lookAt += moveVector;
	if (moveVector.x || moveVector.y || moveVector.z) {
		flag_shouldUpdate = true;
	}
}

void FirstPersonCamera::move(int x, int y, int z) {
	glm::vec3 moveVector(x, y, z);
	position += moveVector;
	lookAt += moveVector;
	if (moveVector.x || moveVector.y || moveVector.z) {
		flag_shouldUpdate = true;
	}
}

void FirstPersonCamera::move(glm::vec3 moveVector) {
	position += moveVector;
	lookAt += moveVector;
	if (moveVector.x || moveVector.y || moveVector.z) {
		flag_shouldUpdate = true;
	}
}

glm::vec3 FirstPersonCamera::getMoveVector(float dx, float dy, float dz) {
	glm::vec3 moveVector(0.f, 0.f, 0.f);
	if (dx || dy || dz) {
		glm::vec3 flatPosition(position.x, 0, position.z);
		glm::vec3 flatLookAt(lookAt.x, 0, lookAt.z);

		glm::vec3 forwardVector = glm::normalize(flatLookAt - flatPosition);
		glm::vec3 rightVector = glm::normalize(glm::cross(forwardVector, glm::vec3(0, 1, 0)));

		moveVector.x = rightVector.x * dx + forwardVector.x * dz;
		moveVector.y = dy;
		moveVector.z = rightVector.z * dx + forwardVector.z * dz;
	}
	return moveVector;
}

void FirstPersonCamera::rotateWithMove(int dx, int dy) {
	if (dx || dy) {
		yaw += (dx * lookSpeed);
		yaw = fmodf(yaw, 360.f);

		float newPitch = pitch + (dy * lookSpeed);
		if (newPitch > 89) {
			pitch = 89;
		} else if (newPitch < -89) {
			pitch = -89;
		} else {
			pitch = newPitch;
		}

		glm::vec3 lookDirection;

		float yawR = MathHelper::toRadians(yaw), pitchR = MathHelper::toRadians(pitch);

		lookDirection.x = MathHelper::cos_float(yawR) * MathHelper::cos_float(pitchR);
		lookDirection.y = MathHelper::sin_float(pitchR);
		lookDirection.z = MathHelper::sin_float(yawR) * MathHelper::cos_float(pitchR);

		lookAt = position + lookDirection;

		flag_shouldUpdate = true;
	}
}

glm::mat4 * FirstPersonCamera::getViewMatrix() {
	if (flag_shouldUpdate) {
		viewMatrix = glm::lookAt(position, lookAt, glm::vec3(0, 1, 0));
		flag_shouldUpdate = false;
	}
	return &viewMatrix;
}

glm::vec3 FirstPersonCamera::getPosition() {
	return position;
}

glm::vec3 FirstPersonCamera::getLook() {
	return lookAt;
}

float FirstPersonCamera::getYaw() {
	return yaw;
}

float FirstPersonCamera::getPitch() {
	return pitch;
}

} /* namespace ts */

