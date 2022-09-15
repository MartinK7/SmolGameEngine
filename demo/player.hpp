
#ifndef SGE_DEMO_PLAYER_HPP
#define SGE_DEMO_PLAYER_HPP

#include "sge/base/camera.hpp"
#include "sge/gl/program.hpp"
#include "sge/gl/window.hpp"

namespace Game {
	class Player {
		SGE::Camera camera;
		glm::vec2 cameraPlayerAzimuthAltitude = glm::vec2(214.0f, -9.0);

	public:
		Player();
		void setUniforms(GL::Program &targetProgram);
		void update(GL::Window &window);
		glm::vec3 getPosition();
		glm::vec3 getRotationVector();
	};
}

#endif //SGE_DEMO_PLAYER_HPP
