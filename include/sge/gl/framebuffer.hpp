
#ifndef SMOLGAMEENGINE_FRAMEBUFFER_HPP
#define SMOLGAMEENGINE_FRAMEBUFFER_HPP

#include "glad.h"
#include "texture.hpp"

namespace GL {

	enum class FramebufferType {
		T2D_DEPTHMAP,
		CUBEMAP_DEPTHMAP,
		T2D_COLOR,
		CUBEMAP_COLOR
	};

	class Framebuffer {
		GLuint ID_fbo = 0;
		GL::Texture textureColor;
		GL::Texture textureDepth;
		FramebufferType type = FramebufferType::T2D_COLOR;

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
