
#include "sge/gl/draw.hpp"

namespace GL::Draw {
	void Draw(VBO::Target target, GLsizei count, Mode mode) {
		switch (target) {
			case VBO::ArrayBuffer:
				glDrawArrays(mode, 0, count);
				break;
			case VBO::ElementArrayBuffer:
				glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
				break;
		}
	}

	void DrawArraysInstanced(GLsizei countVertices, GLsizei countInstances, enum Mode mode) {
		glDrawArraysInstanced(mode, 0, countVertices, countInstances);
	}

	void DrawElementsInstanced(GLsizei countPrimitives, GLsizei countInstances, enum Mode mode) {
		GLsizei modeSize = 0;
		switch (mode) {
			case Triangles:
				modeSize = 3;
				break;
			case Points:
				modeSize = 1;
				break;
			case Lines:
				modeSize = 2;
				break;
			default:
				return;
		}
		glDrawElementsInstanced(mode, countPrimitives * modeSize, GL_UNSIGNED_INT, nullptr, countInstances);
	}
}
