
#ifndef GL_VAO_HPP
#define GL_VAO_HPP

#include "glad.h"

#include <list>

namespace GL {
	namespace VAO {
		enum Attribute {
			Float = 1,
			Vec2f = 2,
			Vec3f = 3,
			Vec4f = 4,
			Mat3f = 9,
			Mat4f = 16
		};
	}

	class VertexArrayObject {
		GLuint ID_VAO = 0; // Attribute buffer
		GLint counter = 0;
	public:
		~VertexArrayObject();
		void createArray();
		void addAttributes(const std::list<VAO::Attribute> &attributes, GLuint divisor = 0);
		void bind() const;
		void unbind() const;
	};
}

#endif //GL_VAO_HPP
