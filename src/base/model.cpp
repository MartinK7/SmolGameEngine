
#include "sge/base/model.hpp"

#include <iostream>
#include <fstream>

class OBJface {
	public:
	uint32_t index_vertex[3];
	uint32_t index_uv[3];
	uint32_t index_normal[3];
};

class OBJobject {
	public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> normals;
	std::vector<OBJface> faces;
};

void SGE::Model::createFromOBJ(std::string objFullPath) {
	std::vector<SGE::Vertex> triangles;

	std::ifstream ifs;
	ifs.open(objFullPath, std::ifstream::in);

	if(ifs.fail()){
		std::cerr << "OBJ file " + std::string(objFullPath) + "Not exist!" << std::endl;
		return;
	}

	OBJobject object;

	std::string word;
	ifs >> word;
	while (ifs.good()) {
		if(word == "v") {
			glm::vec3 vtemp;
			ifs >> vtemp.x;
			ifs >> vtemp.y;
			ifs >> vtemp.z;
			object.vertices.push_back(vtemp);
		} else if(word == "vt") {
			glm::vec2 UVtemp;
			ifs >> UVtemp.x;
			ifs >> UVtemp.y;
			object.UVs.push_back(UVtemp);
		} else if(word == "vn") {
			glm::vec3 vtemp;
			ifs >> vtemp.x;
			ifs >> vtemp.y;
			ifs >> vtemp.z;
			object.normals.push_back(vtemp);
		} else if(word == "f") {
			OBJface ftemp;
			for(uint8_t i=0; i<3; ++i) {
				ifs >> word;
				size_t t = word.find('/');
				ftemp.index_vertex[i] = stoi(word.substr(0, t)) - 1;
				word = word.substr(t + 1);
				t = word.find('/');
				ftemp.index_uv[i] = stoi(word.substr(0, t)) - 1;
				word = word.substr(t + 1);
				ftemp.index_normal[i] = stoi(word) - 1;
			}
			object.faces.push_back(ftemp);
		} else {
			ifs.ignore(256, '\n');
		}
		ifs >> word;
	}
	ifs.close();

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
}

void SGE::Model::createMesh(std::vector<SGE::Vertex> &vertices_triangles) {
	vao.createArray();
	vbo.createBufferData(vertices_triangles, GL::VBO::ArrayBuffer);
	vao.addAttributes({GL::VAO::Vec3f, GL::VAO::Vec3f});
}

void SGE::Model::createCube() {
	std::vector<SGE::Vertex> triangles({
		SGE::Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f),
		SGE::Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f),
		SGE::Vertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f),
		SGE::Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f),
		SGE::Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f),
		SGE::Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f),

		SGE::Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		SGE::Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		SGE::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		SGE::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		SGE::Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		SGE::Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),

		SGE::Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f),
		SGE::Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),
		SGE::Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f),
		SGE::Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f),
		SGE::Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
		SGE::Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f),

		SGE::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		SGE::Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f),
		SGE::Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
		SGE::Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f),
		SGE::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		SGE::Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),

		SGE::Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f),
		SGE::Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f),
		SGE::Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f),
		SGE::Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f),
		SGE::Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f),
		SGE::Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f),

		SGE::Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
		SGE::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		SGE::Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f),
		SGE::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		SGE::Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
		SGE::Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
	});
	vao.createArray();
	vbo.createBufferData(triangles, GL::VBO::ArrayBuffer);
	vao.addAttributes({GL::VAO::Vec3f, GL::VAO::Vec2f, GL::VAO::Vec3f});
}

void SGE::Model::draw() {
	vao.bind();
	vbo.bind();
	if(ebo.getCount()) {
		ebo.bind();
		if(ibo.getCount()) {
			ibo.bind();
			GL::Draw::DrawElementsInstanced(ebo, ibo, GL::Draw::Triangles);
			return;
		}
		GL::Draw::Draw(ebo, GL::Draw::Triangles);
		return;
	}
	GL::Draw::Draw(vbo, GL::Draw::Triangles);
}

void SGE::Model::createPlane() {
	std::vector<SGE::Vertex> triangles({
		SGE::Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		SGE::Vertex(1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		SGE::Vertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		SGE::Vertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		SGE::Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		SGE::Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	});
	vao.createArray();
	vbo.createBufferData(triangles, GL::VBO::ArrayBuffer);
	vao.addAttributes({GL::VAO::Vec3f, GL::VAO::Vec2f, GL::VAO::Vec3f});
}

void SGE::Model::createSphere(GLint sectorCount, GLint stackCount) {
	std::vector<SGE::Vertex> triangles;
	std::vector<GLuint> indices;

	// This algorithm implementation is from:
	// http://www.songho.ca/opengl/gl_sphere.html

	const float radius = 1.0f;

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2.0f * M_PIf32 / (float)sectorCount;
	float stackStep = M_PIf32 / (float)stackCount;
	float sectorAngle, stackAngle;

	for(int i = 0; i <= stackCount; ++i)
	{
		stackAngle = M_PIf32 / 2.0f - (float)i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for(int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = (float)j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / (float)sectorCount;
			t = (float)i / (float)stackCount;

			triangles.emplace_back(x, y, z, s, t, nx, ny, nz);
		}
	}

	// generate CCW index list of sphere triangles
	// k1--k1+1
	// |  / |
	// | /  |
	// k2--k2+1
	int k1, k2;
	for(int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if(i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if(i != (stackCount-1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	vao.createArray();
	vbo.createBufferData(triangles, GL::VBO::ArrayBuffer);
	ebo.createBufferData(indices, GL::VBO::ElementArrayBuffer);
	vao.addAttributes({GL::VAO::Vec3f, GL::VAO::Vec2f, GL::VAO::Vec3f});
}

void SGE::Model::setIndices(std::vector<SGE::Affine> &indices) {
	bool initiated = ibo.getCount();
	ibo.createBufferData(indices, GL::VBO::ArrayBuffer);
	if(!initiated) {
		vao.addAttributes({GL::VAO::Vec3f, GL::VAO::Vec3f, GL::VAO::Vec3f}, 1);
	}
}
