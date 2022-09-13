
#ifndef GL_DRAW_HPP
#define GL_DRAW_HPP

#include "glad.h"
#include "vbo.hpp"

namespace GL::Draw {
	enum Mode {
		Points = GL_POINTS,
		LineStrip = GL_LINE_STRIP,
		LineLoop = GL_LINE_LOOP,
		Lines = GL_LINES,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
		Triangles = GL_TRIANGLES,
		Patches = GL_PATCHES
	};

	void Draw(enum VBO::Target target, GLsizei count, enum Mode mode);
	void DrawElementsInstanced(GLsizei countPrimitives, GLsizei countInstances, enum Mode mode);
	void DrawArraysInstanced(GLsizei countVertices, GLsizei countInstances, enum Mode mode);

	template<typename T>
	inline void Draw(VertexBufferObject<T> &vbo_ebo, enum Mode mode) {
		Draw(vbo_ebo.getTarget(), vbo_ebo.getCount(), mode);
	}

	template<typename T, typename T2>
	inline void DrawElementsInstanced(VertexBufferObject<T> &ebo, VertexBufferObject<T2> &ibo, enum Mode mode) {
		DrawElementsInstanced(ebo.getCount(), ibo.getCount(), mode);
	}

	template<typename T, typename T2>
	inline void DrawArraysInstanced(VertexBufferObject<T> &vbo, VertexBufferObject<T2> &ibo, enum Mode mode) {
		DrawArraysInstanced(vbo.getCount(), ibo.getCount(), mode);
	}
}

#endif //GL_DRAW_HPP
