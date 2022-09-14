
#include "sge/gl/texture.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define OPTIMIZE 1

namespace GL {
#if OPTIMIZE
	GLuint GL_state_textures[32] = {0};
#endif

	void Texture::createFromFile(const char *filename) {
		int _width, _height, _channels;
		void *imagePixels = stbi_load(filename, &_width, &_height, &_channels, 0);

		if(!imagePixels) {
			std::cerr << "Loading error! Texture '" << filename << "' not found!" << std::endl;
			return;
		}

		createFromMemory(imagePixels, _width, _height, _channels);

		stbi_image_free(imagePixels);
	}

	void Texture::createFromMemory(void *imagePixels, GLsizei width, GLsizei height, GLuint channels) {
		if(!imagePixels && channels != 1)
			return;

		if(!ID_texture) {
			// New
			glGenTextures(1, &ID_texture);
			if(!ID_texture)
				return;//error
		}

		type = TextureType::T2D;

		this->width = width;
		this->height = height;

		// Select default texture unit
		glActiveTexture(GL_TEXTURE0);
		// Bind texture to texture unit
		glBindTexture(GL_TEXTURE_2D, ID_texture);
		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, channels == 1 ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, channels == 1 ? GL_NEAREST : GL_LINEAR);
		// Enable clamp depth map
		if(channels == 1) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}
		// Upload data
		if(channels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagePixels);
		} else if(channels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagePixels);
		} else if(channels == 1) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, imagePixels);
		} else {
			fprintf(stderr, "Texture image must be equal to: 1, 3 or 4 channels!\n");
		}
		if(channels != 1) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	void Texture::createCubemapFromFiles(const char *filenames[6]) {
		int widthLast, heightLast, channelsLast;
		void *imagePixels[6];

		for(int i = 0; i < 6; i++) {
			int widthTmp, heightTmp, channelsTmp;
			imagePixels[i] = stbi_load(filenames[i], &widthTmp, &heightTmp, &channelsTmp, 0);

			bool lastCheck = (i != 0) && (widthTmp != widthLast || heightTmp != heightLast || channelsTmp != channelsLast);
			widthLast = widthTmp;
			heightLast = heightTmp;
			channelsLast = channelsTmp;

			if(!imagePixels[i] || lastCheck) {
				std::cerr << "Loading error! Texture '" << filenames[i] << (lastCheck ? "' has not same width/height or channel count as previous!" : " not found!") << std::endl;
				// Clean memory
				for(int j = 0; j < i; j++) {
					stbi_image_free(imagePixels[j]);
				}
				return;
			}
		}

		createCubemapFromMemory(imagePixels, widthLast, heightLast, channelsLast);

		// Clean memory
		for(int i = 0; i < 6; i++) {
			stbi_image_free(imagePixels[i]);
		}
	}

	void Texture::createCubemapFromMemory(void *imagePixels[6], GLsizei width, GLsizei height, GLuint channels) {
		if(!ID_texture) {
			// New
			glGenTextures(1, &ID_texture);
			if(!ID_texture)
				return;//error
		}

		type = TextureType::CUBEMAP;

		this->width = width;
		this->height = height;

		// Select default texture unit
		glActiveTexture(GL_TEXTURE0);
		// Bind texture to texture unit
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID_texture);
		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, channels == 1 ? GL_NEAREST : GL_LINEAR);//GL_LINEAR_MIPMAP_LINEAR
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, channels == 1 ? GL_NEAREST : GL_LINEAR);
		// Enable clamp depth map
		if(channels == 1) {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//			glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor); // Is this needed?
		}
		// Upload data
		if(channels == 3) {
			for(int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagePixels ? imagePixels[i] : nullptr);
			}
		} else if(channels == 4) {
			for(int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagePixels ? imagePixels[i] : nullptr);
			}
		} else if(channels == 1) {
			for(int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, imagePixels ? imagePixels[i] : nullptr);
			}
		} else {
			fprintf(stderr, "Texture image must be equal to: 1, 3 or 4 channels!\n");
			return;
		}
		if(channels != 1) {
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
	}

	void Texture::bind(GLuint layer) const {
		if(layer >= 32)return;
#if OPTIMIZE
		if(GL_state_textures[layer] != ID_texture) {
#endif
			glActiveTexture(GL_TEXTURE0 + layer);
			glBindTexture(type == TextureType::T2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP, ID_texture);
#if OPTIMIZE
			GL_state_textures[layer] = ID_texture;
		}
#endif
	}

	void Texture::unbind(GLuint layer) const {
		if(layer >= 32)return;
#if OPTIMIZE
		if(GL_state_textures[layer] == ID_texture) {
#endif
			glActiveTexture(GL_TEXTURE0 + layer);
			glBindTexture(type == TextureType::T2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP, 0);
#if OPTIMIZE
			GL_state_textures[layer] = 0;
		}
#endif
	}

	Texture::~Texture() {
		if(ID_texture) {
			for(GLuint i=0; i<32; ++i)
				unbind(i);
			glDeleteTextures(1, &ID_texture);
		}
	}

	GLuint Texture::getIdTexture() const {
		return ID_texture;
	}

	GLsizei Texture::getWidth() const {
		return width;
	}

	GLsizei Texture::getHeight() const {
		return height;
	}
}
