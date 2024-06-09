
#include "sge/base/material.hpp"

void SGE::Material::setColorAlbedo(const glm::vec3 &colorAlbedo) {
	Material::colorAlbedo = colorAlbedo;
}

void SGE::Material::setTextureAlbedo(const std::shared_ptr<GL::Texture> &textureAlbedo) {
	Material::textureAlbedo = textureAlbedo;
}

void SGE::Material::setMixColorTextureAlbedo(float mixColorTextureAlbedo) {
	Material::mixColorTextureAlbedo = mixColorTextureAlbedo;
}

void SGE::Material::setGlossy(float glossy) {
	Material::glossy = glossy;
}

void SGE::Material::setTextureNormal(const std::shared_ptr<GL::Texture> &textureNormal) {
	Material::textureNormal = textureNormal;
}

void SGE::Material::setUniforms(GL::Program &targetProgram) {
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
