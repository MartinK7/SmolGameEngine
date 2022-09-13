
#include "sge/gl/program.hpp"

#include <cstdlib>
#include <iostream>

namespace GL {
	static GLuint GL_state_program = 0;

	Program::~Program() {
		if(ID_shaderProgram) {
			unbind();
			glDeleteProgram(ID_shaderProgram);
		}
	}

	void Program::createFromMemory(const char *vertexShaderSource, const char *fragmentShaderSource) {
		// Destroy
		if(ID_shaderProgram) {
			unbind();
			glDeleteProgram(ID_shaderProgram);
		}

		char infoLog[512];
		GLint success;

		GLuint ID_vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(ID_vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(ID_vertexShader);
		glGetShaderiv(ID_vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(ID_vertexShader, 512, NULL, infoLog);
			std::cerr << "Vertex shader " << (vertexShaderName.length() ? ("'" + vertexShaderName + "'") : "")
				<< " compilation error!" << std::endl << infoLog << std::endl;
		}

		GLuint ID_fragmentShader;
		ID_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(ID_fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(ID_fragmentShader);
		glGetShaderiv(ID_fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(ID_fragmentShader, 512, NULL, infoLog);
			std::cerr << "Fragment shader " << (fragmentShaderName.length() ? ("'" + fragmentShaderName + "'") : "")
					  << " compilation error!" << std::endl << infoLog << std::endl;
		}

		ID_shaderProgram = glCreateProgram();
		glAttachShader(ID_shaderProgram, ID_vertexShader);
		glAttachShader(ID_shaderProgram, ID_fragmentShader);
		glLinkProgram(ID_shaderProgram);
		glDeleteShader(ID_vertexShader);
		glDeleteShader(ID_fragmentShader);

		bind();
	}

	void Program::bind() const {
		if(GL_state_program != ID_shaderProgram) {
			glUseProgram(ID_shaderProgram);
			GL_state_program = ID_shaderProgram;
		}
	}

	void Program::unbind() const {
		if(GL_state_program == ID_shaderProgram) {
			glUseProgram(0);
			GL_state_program = 0;
		}
	}

	char *Program::fileLoader(const char *fileName) {
		FILE *f = fopen(fileName, "rb");
		if(!f)
			return nullptr;
		fseek(f, 0, SEEK_END);
		uint32_t fsize = ftell(f);
		fseek(f, 0, SEEK_SET);
		char *buffer = (char*)malloc(fsize+1);
		// Check error
		if(!buffer) {
			fclose(f);
			return nullptr;
		}
		fread(buffer, 1, fsize, f);
		buffer[fsize] = '\0';
		return buffer;
	}

	void Program::createFromFile(const char *vertexShaderSourceFileName, const char *fragmentShaderSourceFileName) {
		vertexShaderName = vertexShaderSourceFileName;
		fragmentShaderName = fragmentShaderSourceFileName;

		char *vert = fileLoader(vertexShaderSourceFileName);
		if(!vert) {
			std::cerr << "Loading error! Vertex shader '" << vertexShaderSourceFileName << "' not found!" << std::endl;
			return;
		}
		char *frag = fileLoader(fragmentShaderSourceFileName);
		if(!frag) {
			free(vert);
			std::cerr << "Loading error! Fragment shader '" << fragmentShaderSourceFileName << "' not found!" << std::endl;
			return;
		}
		createFromMemory(vert, frag);
		free(vert);
		free(frag);
	}

	GLint Program::getUniformLocation(const char *name) {
		if(cache.contains(name)) {
			return cache[name];
		}
		GLint ID = glGetUniformLocation(ID_shaderProgram, name);
		if(ID < 0) {
			std::cerr << "Inside program with vertex shader " << (vertexShaderName.length() ? ("'" + vertexShaderName + "'") : "")
					  << " and fragment shader " << (fragmentShaderName.length() ? ("'" + fragmentShaderName + "'") : "")
					  << " uniform variable '" << name << "' not found!" << std::endl;
		}
		cache[name] = ID;
		return ID;
	}

	void Program::setInt(const char *name, GLint value) {
		bind();
		GLint ID = this->getUniformLocation(name);
		if(ID < 0) {
			return;
		}
		glUniform1i(ID, value);
	}

	void Program::setFloat(const char *name, float value) {
		bind();
		GLint ID = this->getUniformLocation(name);
		if(ID < 0) {
			return;
		}
		glUniform1f(ID, value);
	}

	void Program::setVec3f(const char *name, glm::vec3 value) {
		bind();
		GLint ID = this->getUniformLocation(name);
		if(ID < 0) {
			return;
		}
		glUniform3fv(ID, 1, glm::value_ptr(value));
	}

	void Program::setMat4f(const char *name, glm::mat4 value) {
		bind();
		GLint ID = this->getUniformLocation(name);
		if(ID < 0) {
			return;
		}
		glUniformMatrix4fv(ID, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Program::setMat3f(const char *name, glm::mat3 value) {
		bind();
		GLint ID = this->getUniformLocation(name);
		if(ID < 0) {
			return;
		}
		glUniformMatrix3fv(ID, 1, GL_FALSE, glm::value_ptr(value));
	}
}
