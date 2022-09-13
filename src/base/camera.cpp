
#include "sge/base/camera.hpp"

SGE::Camera::Camera(float aspectRatio, glm::vec3 position, glm::vec3 lookAt, float fov, glm::vec3 upVector) :
		aspectRatio(aspectRatio), position(position), lookAt(lookAt), fov(fov), upVector(upVector) {}

glm::mat4 SGE::Camera::getTransformMatrix() {
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(
			glm::radians(fov),
			aspectRatio,
			0.1f,
			100.0f
	);

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(position, lookAt, upVector);

	return projectionMatrix * viewMatrix;
}

glm::vec3 SGE::Camera::getPosition() {
	return position;
}

void SGE::Camera::setPosition(const glm::vec3 &position) {
	this->position = position;
}

glm::vec3 SGE::Camera::getLookAt() {
	return lookAt;
}

void SGE::Camera::setLookAt(const glm::vec3 &lookAt) {
	this->lookAt = lookAt;
}

float SGE::Camera::getFov() const {
	return fov;
}

void SGE::Camera::setFov(float fov) {
	this->fov = fov;
}

glm::vec3 SGE::Camera::getUpVector() {
	return upVector;
}

void SGE::Camera::setUpVector(const glm::vec3 &upVector) {
	this->upVector = upVector;
}

float SGE::Camera::getAspectRatio() {
	return aspectRatio;
}

void SGE::Camera::setAspectRatio(float aspectRatio) {
	this->aspectRatio = aspectRatio;
}
