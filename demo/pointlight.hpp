
#ifndef SGE_DEMO_POINTLIGHT_HPP
#define SGE_DEMO_POINTLIGHT_HPP

#include <functional>
#include <memory>

#include "sge/gl/framebuffer.hpp"
#include "sge/gl/program.hpp"
#include "sge/base/camera.hpp"

namespace Game {
	class PointLight {
		GL::Framebuffer lightDepthCubemap;
		glm::vec3 position;
		std::shared_ptr<GL::Program> program;
	public:
		PointLight(glm::vec3 lightPointPosition, std::shared_ptr<GL::Program> program);
		void renderDepthCubemap(std::function<void(std::shared_ptr<GL::Program> program)> renderCallback);
		void renderDebug(GL::Program &targetProgram);
		void setUniforms(GL::Program &targetProgram);

		const glm::vec3 &getPosition() const;
		void setPosition(const glm::vec3 &position);
	};
}

#endif //SGE_DEMO_POINTLIGHT_HPP
