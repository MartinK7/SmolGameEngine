
#ifndef GL_TEXTURE_HPP
#define GL_TEXTURE_HPP

#include "glad.h"

namespace GL {
	extern GLuint GL_state_textures[32];

	enum class TextureType {
		T2D,
		CUBEMAP
	};

	class Texture {
		GLuint ID_texture = 0;
		GLsizei width = 0;
		GLsizei height = 0;
		TextureType type = TextureType::T2D;

	public:
		~Texture();
		void createFromFile(const char *filename);
		void createCubemapFromFiles(const char *filenames[6]);

		void createFromMemory(void *imagePixels, GLsizei width, GLsizei height, GLuint channels);
		void createCubemapFromMemory(void *imagePixels[6], GLsizei width, GLsizei height, GLuint channels);

		void bind(GLuint layer) const;
		void unbind(GLuint layer) const;

		GLuint getIdTexture() const;
		GLsizei getWidth() const;
		GLsizei getHeight() const;
	};
}

#endif //GL_TEXTURE_HPP
