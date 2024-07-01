
#include "player.hpp"

Game::Player::Player(glm::vec3 position, glm::vec2 azimuthAltitude) {
	this->azimuthAltitude = azimuthAltitude;
	camera.setPosition(position);
	camera.setAspectRatio(90.0);
}

void Game::Player::setUniforms(GL::Program &targetProgram) {
	targetProgram.setMat4f("matrixCamera", camera.getTransformMatrix());
	targetProgram.setVec3f("cameraPosition", camera.getPosition());
}

void Game::Player::update(GL::Window &window) {
	float speedMax = 0.02f;
	float rotationSpeed = 0.3f;

	azimuthAltitude -= rotationSpeed * window.getMouseDelta();
	while(azimuthAltitude.x > 360.0f) { azimuthAltitude.x -= 360.0f; }
	while(azimuthAltitude.x < -360.0f) { azimuthAltitude.x += 360.0f; }
	if(azimuthAltitude.y > 89.5f) { azimuthAltitude.y = 89.5f; }
	if(azimuthAltitude.y < -89.5f) { azimuthAltitude.y = -89.5f; }

	glm::vec4 dirForwardAzimuth(0.0f, 0.0f, -1.0f, 1.0f);
	glm::vec4 dirForwardAzimuthAltitude = dirForwardAzimuth;
	glm::vec4 dirRight(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 dirUp(0.0f, 1.0f, 0.0f, 1.0f);
	glm::mat4 matrixAzimuth = glm::rotate(glm::mat4(1.0f), glm::radians((float)azimuthAltitude.x), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 matrixAltitude = glm::rotate(matrixAzimuth, glm::radians((float)azimuthAltitude.y), glm::vec3(1.0f, 0.0f, 0.0f));
	dirForwardAzimuth = matrixAzimuth * dirForwardAzimuth;
	dirForwardAzimuthAltitude = matrixAltitude * dirForwardAzimuthAltitude;
	dirRight = matrixAltitude * dirRight;

	if(window.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
		speedMax *= 3.0f;
	}

	if(window.isKeyPressed(GLFW_KEY_W)) {
		camera.setPosition(camera.getPosition() + speedMax * glm::vec3(dirForwardAzimuth));
	}

	if(window.isKeyPressed(GLFW_KEY_S)) {
		camera.setPosition(camera.getPosition() - speedMax * glm::vec3(dirForwardAzimuth));
	}

	if(window.isKeyPressed(GLFW_KEY_A)) {
		camera.setPosition(camera.getPosition() - speedMax * glm::vec3(dirRight));
	}

	if(window.isKeyPressed(GLFW_KEY_D)) {
		camera.setPosition(camera.getPosition() + speedMax * glm::vec3(dirRight));
	}

	if(window.isKeyPressed(GLFW_KEY_SPACE)) {
		camera.setPosition(camera.getPosition() + speedMax * glm::vec3(dirUp));
	}

	if(window.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		camera.setPosition(camera.getPosition() - speedMax * glm::vec3(dirUp));
	}

	camera.setAspectRatio(window.getRatio());
	camera.setLookAt(camera.getPosition() + glm::vec3(dirForwardAzimuthAltitude));
}

glm::vec3 Game::Player::getPosition() {
	return camera.getPosition();
}

glm::vec3 Game::Player::getRotationVector() {
	return glm::normalize(camera.getLookAt() - camera.getPosition());
}
