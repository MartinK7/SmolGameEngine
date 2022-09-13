
#include "sge/gl/vao.hpp"

namespace GL {
	static GLuint GL_state_VAO = 0;

	VertexArrayObject::~VertexArrayObject() {
		if(ID_VAO) {
			unbind();
			glDeleteVertexArrays(1, &ID_VAO);
		}
	}

	void VertexArrayObject::createArray() {
		// Destroy
		if(ID_VAO) {
			unbind();
			glDeleteVertexArrays(1, &ID_VAO);
		}

		glGenVertexArrays(1, &ID_VAO);
		bind();
	}

	void VertexArrayObject::bind() const {
		if(GL_state_VAO != ID_VAO) {
			glBindVertexArray(ID_VAO);
			GL_state_VAO = ID_VAO;
		}
	}

	void VertexArrayObject::unbind() const {
		if(GL_state_VAO == ID_VAO) {
			glBindVertexArray(0);
			GL_state_VAO = 0;
		}
	}

	void VertexArrayObject::addAttributes(const std::list<VAO::Attribute> &attributes, GLuint divisor) {
		bind();
		GLint sum = 0;
		size_t offsetCounter = 0;

		for(auto &k : attributes) {
			sum += k;
		}

		for(auto &k : attributes) {
			switch (k) {
				case VAO::Mat3f:
					for(int i=0; i<3; ++i) {
						glEnableVertexAttribArray(counter);
						glVertexAttribPointer(counter, 3, GL_FLOAT, GL_FALSE, sum * sizeof(float),
											  (void *) offsetCounter);
					}
					offsetCounter += (3*3 * sizeof(float));
					break;
				case VAO::Mat4f:
					for(int i=0; i<4; ++i) {
						glEnableVertexAttribArray(counter);
						glVertexAttribPointer(counter, 4, GL_FLOAT, GL_FALSE, sum * sizeof(float),
											  (void *) offsetCounter);
					}
					offsetCounter += (4*4 * sizeof(float));
					break;
				default:
					glEnableVertexAttribArray(counter);
					glVertexAttribPointer(counter, k, GL_FLOAT, GL_FALSE, sum * sizeof(float), (void*)offsetCounter);
					offsetCounter += (k*sizeof(float));
					break;
			}
			if(divisor)
				glVertexAttribDivisor(counter, divisor);
			counter++;
		}
	}
}
