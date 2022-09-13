
#ifndef SGE_TYPES_HPP
#define SGE_TYPES_HPP

#include "glm/glm.hpp"

namespace SGE {
	class VertexLSTN {
		glm::vec3 position;
		glm::vec2 ST;
		glm::vec3 normal;
	public:
		VertexLSTN(float x, float y, float z, float s, float t, float nx, float ny, float nz) :
				position(glm::vec3(x, y, z)), ST(glm::vec2(s, t)), normal(glm::vec3(nx, ny, nz)) {}
		VertexLSTN(glm::vec3 position, glm::vec2 ST, glm::vec3 normal) : position(position), ST(ST), normal(normal) {}
	};

	class VertexL2ST {
		glm::vec2 position;
		glm::vec2 ST;
	public:
		VertexL2ST(float x, float y, float s, float t) : position(glm::vec2(x, y)), ST(glm::vec2(s, t)) {}
		VertexL2ST(glm::vec2 position, glm::vec2 ST) : position(position), ST(ST) {}
	};
}

#endif //SGE_TYPES_HPP
