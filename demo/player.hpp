
#ifndef SGE_DEMO_PLAYER_HPP
#define SGE_DEMO_PLAYER_HPP

#include "sge/base/camera.hpp"
#include "sge/gl/program.hpp"
#include "sge/gl/window.hpp"

namespace Game {
	class Player {
		SGE::Camera camera;
		glm::vec2 azimuthAltitude;

	public:
		Player(glm::vec3 position = glm::vec3(0.0f), glm::vec2 azimuthAltitude = glm::vec2(0.0f));
		void setUniforms(GL::Program &targetProgram);
		void update(GL::Window &window);
		glm::vec3 getPosition();
		glm::vec3 getRotationVector();
	};
}

#endif //SGE_DEMO_PLAYER_HPP
