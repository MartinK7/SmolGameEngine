
#include "sge/gl/framebuffer.hpp"

GL::Framebuffer::~Framebuffer() {
	if(!ID_fbo) {
		return;
	}
	glDeleteFramebuffers(1, &ID_fbo);
}

void GL::Framebuffer::createFramebufferDepthmap(GLsizei width, GLsizei height) {
	if(!ID_fbo) {
		glGenFramebuffers(1, &ID_fbo);
	}

	// Create empty texture
	textureDepth.createFromMemory(nullptr, width, height, 1);
	type = FramebufferType::T2D_DEPTHMAP;
}

void GL::Framebuffer::createFramebufferCubemapDepthmap(GLsizei width, GLsizei height) {
	if(!ID_fbo) {
		glGenFramebuffers(1, &ID_fbo);
	}

	// Create empty cubemap texture
	bind();
	textureDepth.createCubemapFromMemory(nullptr, width, height, 1);
	type = FramebufferType::CUBEMAP_DEPTHMAP;
}

void GL::Framebuffer::createFramebuffer(GLsizei width, GLsizei height) {
	if(!ID_fbo) {
		glGenFramebuffers(1, &ID_fbo);
	}

	// Create empty texture
	textureColor.createFromMemory(nullptr, width, height, 3);
	textureDepth.createFromMemory(nullptr, width, height, 1);
	type = FramebufferType::T2D_COLOR;
}

void GL::Framebuffer::createFramebufferCubemap(GLsizei width, GLsizei height) {
	if(!ID_fbo) {
		glGenFramebuffers(1, &ID_fbo);
	}

	// Create empty cubemap texture
	textureColor.createCubemapFromMemory(nullptr, width, height, 3);
	textureDepth.createCubemapFromMemory(nullptr, width, height, 1);
	type = FramebufferType::CUBEMAP_COLOR;
}

void GL::Framebuffer::setViewPort(int face) {
	bind();
	switch(type) {
		case FramebufferType::T2D_DEPTHMAP:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepth.getIdTexture(), 0);
#ifndef CONFIG_OPENGL_ES2
			glDrawBuffer(GL_NONE);
#endif // CONFIG_OPENGL_ES2
			break;
		case FramebufferType::CUBEMAP_DEPTHMAP:
			//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, textureDepth.getIdTexture(), 0);
#ifndef CONFIG_OPENGL_ES2
			glDrawBuffer(GL_NONE);
#endif // CONFIG_OPENGL_ES2
			break;
		case FramebufferType::T2D_COLOR:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColor.getIdTexture(), 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepth.getIdTexture(), 0);
			break;
		case FramebufferType::CUBEMAP_COLOR:
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, textureColor.getIdTexture(), 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, textureDepth.getIdTexture(), 0);
			break;
	}
	/*if(face == 0)*/ {
#ifndef CONFIG_OPENGL_ES2
		//
		glReadBuffer(GL_NONE);
#endif // CONFIG_OPENGL_ES2
		//
		glViewport(0, 0, textureDepth.getWidth(), textureDepth.getHeight());
		//
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}
	// Now you can render to texture
}

void GL::Framebuffer::bindTextureColor(GLuint layer) {
	textureColor.bind(layer);
}

void GL::Framebuffer::bindTextureDepth(GLuint layer) {
	textureDepth.bind(layer);
}
#include <cstdio>
void GL::Framebuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, ID_fbo);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		// Handle incomplete framebuffer
		printf("Framebuffer not complete: %d %X\n", ID_fbo, status);
	}
}
