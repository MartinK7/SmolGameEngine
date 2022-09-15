
#ifndef SGE_TYPES_HPP
#define SGE_TYPES_HPP

#include "glm/glm.hpp"

namespace SGE {
	class Vertex {
		glm::vec3 position;
		glm::vec2 ST;
		glm::vec3 normal;
	public:
		Vertex(float x, float y, float z, float s, float t, float nx, float ny, float nz) :
				position(glm::vec3(x, y, z)), ST(glm::vec2(s, t)), normal(glm::vec3(nx, ny, nz)) {}
		Vertex(glm::vec3 position, glm::vec2 ST, glm::vec3 normal) : position(position), ST(ST), normal(normal) {}
	};
}

#endif //SGE_TYPES_HPP
