
#ifndef SMOLGAMEENGINE_CAMERA_HPP
#define SMOLGAMEENGINE_CAMERA_HPP

#include "../gl/gl.hpp"

namespace SGE {
	class Camera {
		glm::vec3 position;
		glm::vec3 lookAt;
		glm::vec3 upVector;
		float fov;
		float aspectRatio;
	public:
		float getAspectRatio();
		void setAspectRatio(float aspectRatio);
		glm::vec3 getUpVector();
		void setUpVector(const glm::vec3 &upVector);
		glm::vec3 getPosition();
		glm::vec3 getLookAt();

		float getFov() const;
		void setPosition(const glm::vec3 &position);
		void setLookAt(const glm::vec3 &lookAt);
		void setFov(float fov);

		explicit Camera(float aspectRatio = 1.0f, glm::vec3 position = glm::vec3(0.0f), glm::vec3 lookAt = glm::vec3(0.0f), float fov = 90.0f, glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 getTransformMatrix();
	};
}

#endif //SMOLGAMEENGINE_CAMERA_HPP
