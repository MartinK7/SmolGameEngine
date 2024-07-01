
#include <fstream>

#include "pointlight.hpp"
#include "sge/base/affine.hpp"
#include "sge/base/model.hpp"

Game::PointLight::PointLight(glm::vec3 lightPointPosition, std::shared_ptr<GL::Program> programPointLight) {
	this->position = lightPointPosition;
	this->program = programPointLight;

	lightDepthCubemap.createFramebufferCubemapDepthmap(1024, 1024); // This bugs and hide other textures, why??
	lightDepthCubemap.bindTextureDepth(10);
}

const glm::vec3 &Game::PointLight::getPosition() const {
	return position;
}

void Game::PointLight::setPosition(const glm::vec3 &position) {
	PointLight::position = position;
}

void Game::PointLight::renderDepthCubemap(std::function<void(std::shared_ptr<GL::Program> program)> renderCallback) {
	SGE::Camera cameraCubemap(1.0f, glm::vec3(0.0), glm::vec3(0.0), 90.0f);
	std::array<std::pair<glm::vec3, glm::vec3>, 6> cameraCubemapList = {
			std::make_pair(glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.0,-1.0, 0.0)),
			std::make_pair(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0,-1.0, 0.0)),
			std::make_pair(glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0)),
			std::make_pair(glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0, 0.0,-1.0)),
			std::make_pair(glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0,-1.0, 0.0)),
			std::make_pair(glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0,-1.0, 0.0))
	};
	// Render light depth cubemap
	cameraCubemap.setPosition(PointLight::position);
	program->setVec3f("sge_pointLight.position", PointLight::position);
	for(int i = 0; i < 6; ++i) {
		// Bind target cubemap face
		lightDepthCubemap.setViewPort(i);

		// Camera
		cameraCubemap.setLookAt(cameraCubemap.getPosition() + cameraCubemapList[i].first);
		cameraCubemap.setUpVector(cameraCubemapList[i].second);
		program->setMat4f("matrixCamera", cameraCubemap.getTransformMatrix());

		renderCallback(program);
	}
}

void Game::PointLight::setUniforms(GL::Program &targetProgram) {
	targetProgram.setVec3f("sge_pointLight.position", position);
}

void Game::PointLight::renderDebug(GL::Program &targetProgram) {
	SGE::Affine transform;
	transform.reset();
	transform.setScale(glm::vec3(0.01f));
	transform.setPosition(position);
	targetProgram.setMat4f("matrixModel", transform.getTransformMatrix());
	SGE::Model modelCube;
	modelCube.createCube();
	modelCube.draw();
}
