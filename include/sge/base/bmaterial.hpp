
#ifndef SMOLGAMEENGINE_BMATERIAL_HPP
#define SMOLGAMEENGINE_BMATERIAL_HPP

#include <memory>
#include "../gl/texture.hpp"
#include "../gl/program.hpp"
#include <glm/glm.hpp>

namespace SGE {
	class BMaterial {
		glm::vec3 colorAlbedo = glm::vec3(0.5);
		std::shared_ptr<GL::Texture> textureAlbedo;
		std::shared_ptr<GL::Texture> textureNormal;
		float mixColorTextureAlbedo = 0.0f;
		float glossy = 0.0f;

	public:
		void setColorAlbedo(const glm::vec3 &colorAlbedo);
		void setTextureAlbedo(const std::shared_ptr<GL::Texture> &textureAlbedo);
		void setTextureNormal(const std::shared_ptr<GL::Texture> &textureNormal);
		void setMixColorTextureAlbedo(float mixColorTextureAlbedo);
		void setGlossy(float glossy);

		void setUniforms(GL::Program &targetProgram);
	};
}

#endif //SMOLGAMEENGINE_BMATERIAL_HPP
