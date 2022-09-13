
#ifndef GL_PROGRAM_HPP
#define GL_PROGRAM_HPP

#include "glad.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

namespace GL {
	class Program {
		std::map<std::string, GLint> cache;
		GLuint ID_shaderProgram = 0;
		char* fileLoader(const char *fileName);
		GLint getUniformLocation(const char *name);
		std::string vertexShaderName, fragmentShaderName;
	public:
		~Program();
		void createFromFile(const char *vertexShaderSourceFileName, const char *fragmentShaderSourceFileName);
		void createFromMemory(const char *vertexShaderSource, const char *fragmentShaderSource);

		void bind() const;
		void unbind() const;

		void setInt(const char *name, GLint value);
		void setFloat(const char *name, float value);
		void setVec3f(const char *name, glm::vec3 value);
		void setMat4f(const char *name, glm::mat4 value);
		void setMat3f(const char *name, glm::mat3 value);
	};
}

#endif //GL_PROGRAM_HPP
