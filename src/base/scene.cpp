
#include <iostream>
#include <fstream>

#include "sge/base/scene.hpp"

void SGE::Scene::loadFromFile(std::string &sceneFullPath) {

	std::ifstream ifs;
	ifs.open(sceneFullPath, std::ifstream::in);

	if(ifs.fail()){
		std::cerr << "SCE file " + std::string(sceneFullPath) + "Not exist!" << std::endl;
		return;
	}

	std::string word;
	while(ifs.good()) {
		ifs >> word;
		// Comment
		if(word == "#") {
			// Ignore rest of the file
			ifs.ignore(256, '\n');
			continue;
		}
		if(word == "model") {
			continue;
		}
		std::cerr << "SCE file " + std::string(sceneFullPath) + "Not exist!" << std::endl;

	}
	ifs.close();
/*
	// REORDER DATA
	for(auto face : object.faces) {
		for (uint8_t faceIndex = 0; faceIndex < 3; ++faceIndex) {
			SGE::Vertex vertex(
					object.vertices[face.index_vertex[faceIndex]].x,
					object.vertices[face.index_vertex[faceIndex]].y,
					object.vertices[face.index_vertex[faceIndex]].z,
					object.UVs[face.index_uv[faceIndex]].x,
					object.UVs[face.index_uv[faceIndex]].y,
					object.normals[face.index_normal[faceIndex]].x,
					object.normals[face.index_normal[faceIndex]].y,
					object.normals[face.index_normal[faceIndex]].z
			);
			triangles.push_back(vertex);
		}
	}

	vao.createArray();
	vbo.createBufferData(triangles, GL::VBO::ArrayBuffer);
	vao.addAttributes({GL::VAO::Vec3f, GL::VAO::Vec2f, GL::VAO::Vec3f});
*/}
