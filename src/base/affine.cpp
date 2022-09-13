
#include "sge/base/affine.hpp"

SGE::Affine::Affine(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : position(position), rotation(rotation), scale(scale) {}

glm::mat4 SGE::Affine::getTransformMatrix() {
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, position);
	matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::scale(matrix, scale);
	return matrix;
}

glm::mat3 SGE::Affine::getNormalMatrix() {
	return glm::mat3(glm::transpose(glm::inverse(getTransformMatrix())));
}

const glm::vec3 &SGE::Affine::getPosition() const {
	return position;
}

void SGE::Affine::setPosition(const glm::vec3 &position) {
	Affine::position = position;
}

const glm::vec3 &SGE::Affine::getRotation() const {
	return rotation;
}

void SGE::Affine::setRotation(const glm::vec3 &rotation) {
	Affine::rotation = rotation;
}

const glm::vec3 &SGE::Affine::getScale() const {
	return scale;
}

void SGE::Affine::setScale(const glm::vec3 &scale) {
	Affine::scale = scale;
}

void SGE::Affine::reset() {
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
}

void SGE::Affine::setUniforms(GL::Program &targetProgram) {
	targetProgram.setMat4f("matrixModel", getTransformMatrix());
}
