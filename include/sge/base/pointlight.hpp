
#ifndef SMOLGAMEENGINE_POINTLIGHT_HPP
#define SMOLGAMEENGINE_POINTLIGHT_HPP

#include "../gl/texture.hpp"
#include <glm/glm.hpp>

namespace SGE {
	class PointLight {
		GL::Texture cubemapDepthmap;
		glm::vec3 color;
		float brightness;
	};
}

#endif //SMOLGAMEENGINE_POINTLIGHT_HPP
