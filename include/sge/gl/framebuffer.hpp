
#ifndef SMOLGAMEENGINE_FRAMEBUFFER_HPP
#define SMOLGAMEENGINE_FRAMEBUFFER_HPP

#include "glad.h"
#include "texture.hpp"

namespace GL {

	typedef enum {
		FRAMEBUFFER_TEXTURE_2D_DEPTHMAP,
		FRAMEBUFFER_TEXTURE_CUBEMAP_DEPTHMAP,
		FRAMEBUFFER_TEXTURE_2D_COLOR,
		FRAMEBUFFER_TEXTURE_CUBEMAP_COLOR
	} framebuffer_type_e;

	class Framebuffer {
		GLuint ID_fbo = 0;
		GL::Texture textureColor;
		GL::Texture textureDepth;
		framebuffer_type_e type = FRAMEBUFFER_TEXTURE_2D_COLOR;

	public:
		~Framebuffer();
		void createFramebufferDepthmap(GLsizei width = 1024, GLsizei height = 1024);
		void createFramebufferCubemapDepthmap(GLsizei width = 1024, GLsizei height = 1024);
		void createFramebuffer(GLsizei width = 1024, GLsizei height = 1024);
		void createFramebufferCubemap(GLsizei width = 1024, GLsizei height = 1024);

		void setViewPort(int face = 0);
		void bindTextureColor(GLuint layer);
		void bindTextureDepth(GLuint layer);
		void bind() const;
	};
}

#endif //SMOLGAMEENGINE_FRAMEBUFFER_HPP
