
#ifndef GL_VBO_HPP
#define GL_VBO_HPP

#include "glad.h"
#include <vector>
#include <list>
#include <array>

namespace GL {
	namespace VBO {
		enum Target {
			ArrayBuffer = GL_ARRAY_BUFFER, // Vertex attributes
			ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER // Vertex array indices
		};
	}

	extern GLuint GL_state_VBO;

	template <typename T>
	class VertexBufferObject {
		GLuint ID_VBO = 0; // (Vertices buffer)
		GLuint verticesCount = 0;
		VBO::Target target = VBO::ArrayBuffer;

	public:
		GLuint getCount();
		VBO::Target getTarget();
		void createBufferData(std::vector<T> &vertices, VBO::Target _target);
		~VertexBufferObject();
		void bind();
		void unbind();
	};

	template<typename T>
	GLuint VertexBufferObject<T>::getCount() {
		return verticesCount;
	}

	template<typename T>
	VBO::Target VertexBufferObject<T>::getTarget() {
		return target;
	}

	template<typename T>
	void VertexBufferObject<T>::createBufferData(std::vector<T> &vertices, VBO::Target _target) {
		target = _target;
		if(!ID_VBO) {
			// New
			glGenBuffers(1, &ID_VBO);
		}
		// Reload
		bind();
		glBufferData(target, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
		verticesCount = vertices.size();
	}

	template<typename T>
	VertexBufferObject<T>::~VertexBufferObject() {
		if(ID_VBO) {
			unbind();
			glDeleteBuffers(1, &ID_VBO);
		}
	}

	template<typename T>
	void VertexBufferObject<T>::bind() {
		if(GL_state_VBO != ID_VBO) {
			glBindBuffer(target, ID_VBO);
			GL_state_VBO = ID_VBO;
		}
	}

	template<typename T>
	void VertexBufferObject<T>::unbind() {
		if(GL_state_VBO == ID_VBO) {
			glBindBuffer(target, 0);
			GL_state_VBO = 0;
		}
	}
}

#endif //GL_VBO_HPP
