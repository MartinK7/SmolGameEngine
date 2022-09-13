
#ifndef SMOLGAMEENGINE_AFFINETRANSFORM_HPP
#define SMOLGAMEENGINE_AFFINETRANSFORM_HPP

#include "../gl/program.hpp"

namespace SGE {
	class Affine {
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	public:
		const glm::vec3 &getPosition() const;
		const glm::vec3 &getRotation() const;
		const glm::vec3 &getScale() const;
		void setPosition(const glm::vec3 &position);
		void setRotation(const glm::vec3 &rotation);
		void setScale(const glm::vec3 &scale);

		void reset();

		explicit Affine(
			glm::vec3 position = glm::vec3(0.0f),
			glm::vec3 rotation = glm::vec3(0.0f),
			glm::vec3 scale = glm::vec3(1.0f)
		);
		glm::mat4 getTransformMatrix();
		glm::mat3 getNormalMatrix();

		void setUniforms(GL::Program &targetProgram);
	};
}

#endif //SMOLGAMEENGINE_AFFINETRANSFORM_HPP
