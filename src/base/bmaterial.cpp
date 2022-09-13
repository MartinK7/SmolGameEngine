
#include "sge/base/bmaterial.hpp"

void SGE::BMaterial::setColorAlbedo(const glm::vec3 &colorAlbedo) {
	BMaterial::colorAlbedo = colorAlbedo;
}

void SGE::BMaterial::setTextureAlbedo(const std::shared_ptr<GL::Texture> &textureAlbedo) {
	BMaterial::textureAlbedo = textureAlbedo;
}

void SGE::BMaterial::setMixColorTextureAlbedo(float mixColorTextureAlbedo) {
	BMaterial::mixColorTextureAlbedo = mixColorTextureAlbedo;
}

void SGE::BMaterial::setGlossy(float glossy) {
	BMaterial::glossy = glossy;
}

void SGE::BMaterial::setTextureNormal(const std::shared_ptr<GL::Texture> &textureNormal) {
	BMaterial::textureNormal = textureNormal;
}

void SGE::BMaterial::setUniforms(GL::Program &targetProgram) {
	if(textureAlbedo) {
		textureAlbedo->bind(1);
	}
	if(textureNormal) {
		textureNormal->bind(2);
	}

	targetProgram.setVec3f("sge_material.colorAlbedo", colorAlbedo);
	targetProgram.setInt("sge_material.textureAlbedo", 1);
	targetProgram.setInt("sge_material.textureNormal", 2);
	targetProgram.setFloat("sge_material.mixColorTextureAlbedo", mixColorTextureAlbedo);
	targetProgram.setFloat("sge_material.glossy", glossy);
}
