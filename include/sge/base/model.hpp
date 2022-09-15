
#ifndef SGE_SIMPLE_MODEL_HPP
#define SGE_SIMPLE_MODEL_HPP

#include <vector>
#include <memory>

#include "../gl/gl.hpp"
#include "vertex.hpp"
#include "affine.hpp"

namespace SGE {
		class Model {
			GL::VertexArrayObject vao;
			GL::VertexBufferObject<SGE::Vertex> vbo;
			GL::VertexBufferObject<GLuint> ebo;
			GL::VertexBufferObject<SGE::Affine> ibo;

		public:
			void createFromOBJ(std::string objFullPath);
			void createCube();
			void createPlane();
			void createSphere(GLint sectorCount = 16, GLint stackCount = 16);
			void createMesh(std::vector<SGE::Vertex> &vertices_triangles);
			void draw();
			void setIndices(std::vector<SGE::Affine> &indices);
		};
	}

#endif //SGE_SIMPLE_MODEL_HPP
