
#include "demo.hpp"

#include <ctime>
#include <random>
#include <iostream>
#include "glad.h"
#include "sge/gl/gl.hpp"
#include "sge/base/sge.hpp"

#define SCR_WIDTH  640
#define SCR_HEIGHT 480

namespace GL::Test {

	static const uint32_t time_delay = 5;

	class VertexXYST {
	public:
		glm::vec3 position;
		glm::vec2 ST;
		VertexXYST(glm::vec3 position, glm::vec2 ST) : position(position), ST(ST) {}
	};

	static void model_normal() {
		GL::Window win;
		win.create(SCR_WIDTH, SCR_HEIGHT);

		const float diameter = 0.1f;
		const uint32_t count = 50000;
		std::vector<VertexXYST> vertices;
		vertices.emplace_back(VertexXYST(glm::vec3(-diameter, -diameter, 0),glm::vec2(1, 1)));
		vertices.emplace_back(VertexXYST(glm::vec3(diameter, -diameter, 0),glm::vec2(0, 1)));
		vertices.emplace_back(VertexXYST(glm::vec3(diameter, diameter, 0),glm::vec2(0, 0)));
		vertices.emplace_back(VertexXYST(glm::vec3(-diameter, diameter, 0),glm::vec2(1, 0)));

		std::vector<GLuint> indices;
		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(2);
		indices.emplace_back(3);
		indices.emplace_back(0);

		GL::VertexArrayObject vao;
		GL::VertexBufferObject<VertexXYST> vbo;
		GL::VertexBufferObject<GLuint> ebo;

		vao.createArray();
		vbo.createBufferData(vertices, GL::VBO::ArrayBuffer);
		ebo.createBufferData(indices, GL::VBO::ElementArrayBuffer);
		vao.addAttributes({GL::VAO::Vec3f, GL::VAO::Vec2f});

		GL::Program program;
		program.createFromFile(CONFIG_ASSETS_PATH"programs/demo/simple.vert", CONFIG_ASSETS_PATH"programs/demo/simple.frag");
		program.setInt("textureAlbedo1", 0);
		program.setInt("textureAlbedo2", 1);

		GL::Texture tex2;
		tex2.createFromFile(CONFIG_ASSETS_PATH"images/textures/TexturesCom_Stone_BaseSurface3_1x1_1K/TexturesCom_Stone_BaseSurface3_1x1_1K_albedo.png");

		GL::Texture tex;
		tex.createFromFile(CONFIG_ASSETS_PATH"images/textures/TexturesCom_Cargo0097/TexturesCom_Cargo0097_M.jpg");

		tex2.bind(1);
		tex.bind(0);

		time_t t = time(nullptr) + time_delay;
		while(!win.shouldExit() && time(nullptr) < t) {
			win.setTitleFPS("");
			// Clear area (background color) with gray color
			win.clear(glm::vec3(0.5f));
			// Draw
			for(uint32_t i = 0; i < count; ++i) {
				GL::Draw::Draw(ebo, GL::Draw::Triangles);
			}
			// Swap
			win.swapBuffer();
		}
	}

	static void model_array() {
		GL::Window win;
		win.create(SCR_WIDTH, SCR_HEIGHT);

		const float diameter = 0.1f;
		const uint32_t count = 50000;
		std::vector<VertexXYST> vertices;
		vertices.emplace_back(VertexXYST(glm::vec3(-diameter, -diameter, 0),glm::vec2(1, 1)));
		vertices.emplace_back(VertexXYST(glm::vec3(diameter, -diameter, 0),glm::vec2(0, 1)));
		vertices.emplace_back(VertexXYST(glm::vec3(diameter, diameter, 0),glm::vec2(0, 0)));
		vertices.emplace_back(VertexXYST(glm::vec3(-diameter, diameter, 0),glm::vec2(1, 0)));

		std::vector<GLuint> indices;
		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(2);
		indices.emplace_back(3);
		indices.emplace_back(0);

		GL::VertexArrayObject vao;
		GL::VertexBufferObject<VertexXYST> vbo;
		GL::VertexBufferObject<GLuint> ebo;

		vao.createArray();
		vbo.createBufferData(vertices, GL::VBO::ArrayBuffer);
		ebo.createBufferData(indices, GL::VBO::ElementArrayBuffer);
		vao.addAttributes({GL::VAO::Vec3f, GL::VAO::Vec2f});

		GL::Program program;
		program.createFromFile(CONFIG_ASSETS_PATH"programs/demo/simple.vert", CONFIG_ASSETS_PATH"programs/demo/simple.frag");
		program.setInt("textureAlbedo1", 0);
		program.setInt("textureAlbedo2", 1);

		GL::Texture tex2;
		tex2.createFromFile(CONFIG_ASSETS_PATH"images/textures/TexturesCom_Stone_BaseSurface3_1x1_1K/TexturesCom_Stone_BaseSurface3_1x1_1K_albedo.png");

		GL::Texture tex;
		tex.createFromFile(CONFIG_ASSETS_PATH"images/textures/TexturesCom_Cargo0097/TexturesCom_Cargo0097_M.jpg");

		GL::VertexBufferObject<glm::vec3> ibo;
		{
			std::vector<glm::vec3> positions;
			positions.reserve(positions.size() * count);
			for(uint32_t i = 0; i < count; ++i) {
				positions.emplace_back(glm::vec3(
						(float)random() / (float)RAND_MAX * 2.0f - 1.0f,
						(float)random() / (float)RAND_MAX * 2.0f - 1.0f,
						(float)random() / (float)RAND_MAX * 2.0f - 1.0f
				));
			}
			ibo.createBufferData(positions, GL::VBO::ArrayBuffer);
		}
		vao.addAttributes({GL::VAO::Vec3f}, 1);

		tex2.bind(1);
		tex.bind(0);

		time_t t = time(nullptr) + time_delay;
		while(!win.shouldExit() && time(nullptr) < t) {
			win.setTitleFPS("");
			// Clear area (background color) with gray color
			win.clear(glm::vec3(0.5f));
			// Draw
			GL::Draw::DrawElementsInstanced(ebo, ibo, GL::Draw::Triangles);
			// Swap
			win.swapBuffer();
		}
	}

	static void two3D() {
		GL::Window win;
		win.create(SCR_WIDTH, SCR_HEIGHT);

		GL::Program program;
		GL::VertexArrayObject vao1, vao2;
		GL::VertexBufferObject<VertexXYST> vbo1, vbo2;
		GL::VertexBufferObject<GLuint> ebo1, ebo2;

		program.createFromFile(CONFIG_ASSETS_PATH"programs/demo/simple.vert", CONFIG_ASSETS_PATH"programs/demo/simple.frag");
		program.setInt("textureAlbedo1", 0);
		program.setInt("textureAlbedo2", 1);

		GL::Texture tex2;
		tex2.createFromFile(CONFIG_ASSETS_PATH"images/textures/TexturesCom_Stone_BaseSurface3_1x1_1K/TexturesCom_Stone_BaseSurface3_1x1_1K_albedo.png");

		GL::Texture tex;
		tex.createFromFile(CONFIG_ASSETS_PATH"images/textures/TexturesCom_Cargo0097/TexturesCom_Cargo0097_M.jpg");

		{
			const float diameter = 0.1f;
			std::vector<VertexXYST> vertices;
			vertices.emplace_back(VertexXYST(glm::vec3(-diameter,  diameter, -diameter),glm::vec2(1, 1)));
			vertices.emplace_back(VertexXYST(glm::vec3(-diameter,  diameter,  diameter),glm::vec2(0, 1)));
			vertices.emplace_back(VertexXYST(glm::vec3( diameter,  diameter,  diameter),glm::vec2(0, 0)));
			vertices.emplace_back(VertexXYST(glm::vec3( diameter,  diameter, -diameter),glm::vec2(1, 0)));
			vertices.emplace_back(VertexXYST(glm::vec3( diameter, -diameter, -diameter),glm::vec2(1, 1)));
			vertices.emplace_back(VertexXYST(glm::vec3( diameter, -diameter,  diameter),glm::vec2(0, 1)));
			vertices.emplace_back(VertexXYST(glm::vec3(-diameter, -diameter,  diameter),glm::vec2(0, 0)));
			vertices.emplace_back(VertexXYST(glm::vec3(-diameter, -diameter, -diameter),glm::vec2(1, 0)));

			for(auto &k : vertices) {
				glm::mat4 transformMatrix;

				transformMatrix = glm::mat4(1.0f);
//				transformMatrix = glm::translate(transformMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
				transformMatrix = glm::rotate(transformMatrix, 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));//z
				transformMatrix = glm::rotate(transformMatrix, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));//y
				transformMatrix = glm::rotate(transformMatrix, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));//x
//				transformMatrix = glm::scale(transformMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

				k.position = transformMatrix * glm::vec4(k.position, 1.0f);
			}

			std::vector<GLuint> firstStructIndices;
			firstStructIndices.emplace_back(0);//Down
			firstStructIndices.emplace_back(1);
			firstStructIndices.emplace_back(2);
			firstStructIndices.emplace_back(2);
			firstStructIndices.emplace_back(3);
			firstStructIndices.emplace_back(0);
			firstStructIndices.emplace_back(4);//Up
			firstStructIndices.emplace_back(5);
			firstStructIndices.emplace_back(6);
			firstStructIndices.emplace_back(6);
			firstStructIndices.emplace_back(7);
			firstStructIndices.emplace_back(4);

			vao1.createArray();
			vbo1.createBufferData(vertices, GL::VBO::ArrayBuffer);
			ebo1.createBufferData(firstStructIndices, GL::VBO::ElementArrayBuffer);
			vao1.addAttributes({GL::VAO::Vec3f, GL::VAO::Vec2f});
		}

		{
			const float diameter = 0.1f;
			std::vector<VertexXYST> vertices;
			vertices.emplace_back(VertexXYST(glm::vec3(-diameter + 0.5f, -diameter, 0),glm::vec2(1, 1)));
			vertices.emplace_back(VertexXYST(glm::vec3(diameter + 0.5f, -diameter, 0),glm::vec2(0, 1)));
			vertices.emplace_back(VertexXYST(glm::vec3(diameter + 0.5f, diameter, 0),glm::vec2(0, 0)));
			vertices.emplace_back(VertexXYST(glm::vec3(-diameter + 0.5f, diameter, 0),glm::vec2(1, 0)));

			std::vector<GLuint> secondStructIndices;
			secondStructIndices.emplace_back(0);
			secondStructIndices.emplace_back(1);
			secondStructIndices.emplace_back(2);
			secondStructIndices.emplace_back(2);
			secondStructIndices.emplace_back(3);
			secondStructIndices.emplace_back(0);

			vao2.createArray();
			vbo2.createBufferData(vertices, GL::VBO::ArrayBuffer);
			ebo2.createBufferData(secondStructIndices, GL::VBO::ElementArrayBuffer);
			vao2.addAttributes({GL::VAO::Vec3f, GL::VAO::Vec2f});
		}

		time_t t = time(nullptr) + time_delay;
		while(!win.shouldExit() && time(nullptr) < t) {
			win.setTitleFPS("");
			// Clear area (background color) with gray color
			win.clear(glm::vec3(0.5f));
			// Draw
			tex2.bind(1);
			tex.bind(0);
			vao1.bind();
			vbo1.bind();
			ebo1.bind();
			GL::Draw::Draw(ebo1, GL::Draw::Triangles);
			// Draw
			tex2.bind(0);
			tex.bind(1);
			vao2.bind();
			vbo2.bind();
			ebo2.bind();
			GL::Draw::Draw(ebo2, GL::Draw::Triangles);
			// Swap
			win.swapBuffer();
		}
	}

	static void cubes() {

#define MSAA 0//4
		GL::Window win;
		win.create(SCR_WIDTH, SCR_HEIGHT, "", MSAA);

		GL::Program program;
		program.createFromFile(CONFIG_ASSETS_PATH"programs/demo/cubes.vert", CONFIG_ASSETS_PATH"programs/demo/cubes.frag");
		program.setInt("textureAlbedo", 0);

		GL::Texture tex;
		tex.createFromFile(CONFIG_ASSETS_PATH"images/textures/TexturesCom_Cargo0097/TexturesCom_Cargo0097_M.jpg");
		tex.bind(0);

		SGE::Model cube;
		cube.createCube();

#if MSAA
		GL::VertexArrayObject quadVAO;
		GL::VertexBufferObject<SGE::VertexP2ST> quadVBO;
		unsigned int framebuffer;
		unsigned int intermediateFBO;
		unsigned int screenTexture;
		GL::Program screenShader;
		{
			std::vector<SGE::VertexP2ST> triangles({
				SGE::VertexP2ST(-1.0f,  1.0f, 0.0f, 1.0f),
				SGE::VertexP2ST(-1.0f, -1.0f, 0.0f, 0.0f),
				SGE::VertexP2ST(1.0f, -1.0f, 1.0f, 0.0f),

				SGE::VertexP2ST(-1.0f,  1.0f, 0.0f, 1.0f),
				SGE::VertexP2ST(1.0f, -1.0f, 1.0f, 0.0f),
				SGE::VertexP2ST(1.0f,  1.0f, 1.0f, 1.0f)
			});
			quadVAO.createArray();
			quadVBO.createBufferData(triangles, GL::VBO::ArrayBuffer);
			quadVAO.addAttributes({GL::VAO::Vec2f, GL::VAO::Vec2f});

			// configure MSAA framebuffer
			// --------------------------
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			// create a multisampled color attachment texture
			unsigned int textureColorBufferMultiSampled;
			glGenTextures(1, &textureColorBufferMultiSampled);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
			// create a (also multisampled) renderbuffer object for depth and stencil attachments
			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// configure second post-processing framebuffer
			glGenFramebuffers(1, &intermediateFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
			// create a color attachment texture
			glGenTextures(1, &screenTexture);
			glBindTexture(GL_TEXTURE_2D, screenTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cerr << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			screenShader.createFromFile(CONFIG_ASSETS_PATH"programs/demo/screen.vert", CONFIG_ASSETS_PATH"programs/demo/screen.frag");
			screenShader.setInt("textureScreen", 0);
		}
#endif // MSAA

		time_t t = time(nullptr) + time_delay;
		while(!win.shouldExit() && time(nullptr) < t) {
			win.setTitleFPS("");
#if MSAA
			// 1. draw scene as normal in multisampled buffers
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			GL_state_textures[0] = 0;
#endif // MSAA
			win.clear(glm::vec3(0.5f));
			//
			glm::mat4 projectionMatrix = glm::mat4(1.0f);
			projectionMatrix = glm::perspective(
				glm::radians(45.0f),
				(float)SCR_WIDTH / (float)SCR_HEIGHT,
				0.1f,
				1000.0f
			);
			//
			glm::mat4 viewMatrix = glm::mat4(1.0f);
			viewMatrix = glm::lookAt(
				glm::vec3(3.0f, 2.0f, 5.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
//			viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
			//
			srandom(4526);
			for(uint32_t i = 0; i < 100; ++i) {
				glm::mat4 transformAffineMatrix = glm::mat4(1.0f);
				transformAffineMatrix = glm::translate(transformAffineMatrix, glm::vec3(
						(float)random() / (float)RAND_MAX * 4.0f - 2.0f,
						(float)random() / (float)RAND_MAX * 4.0f - 2.0f,
						(float)random() / (float)RAND_MAX * 4.0f - 2.0f
				));
//				transformAffineMatrix = glm::rotate(transformAffineMatrix, this->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
				transformAffineMatrix = glm::rotate(transformAffineMatrix, win.getTimeSeconds() * 2.0f * M_PIf32 / 8, glm::vec3(0.0f, 1.0f, 0.0f));
//				transformAffineMatrix = glm::rotate(transformAffineMatrix, this->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
				transformAffineMatrix = glm::scale(transformAffineMatrix, glm::vec3(0.1f));
				//
				// Upload new matrices
				program.bind();
				program.setMat4f("transformAffineMatrix", transformAffineMatrix);
				program.setMat4f("transformCameraMatrix", projectionMatrix * viewMatrix);
				tex.bind(0);
				//
				cube.draw();
			}

#if MSAA
			// 2. now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
			glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
			glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

			// 3. now render quad with scene's visuals as its texture image
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);

			// draw Screen quad
			screenShader.bind();
			quadVAO.bind();
			quadVBO.bind();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, screenTexture);
			GL::Draw::Draw(quadVBO, GL::Draw::Triangles);
#endif // MSAA

			win.swapBuffer();
		}
	}

	static GL::Program programBricks, programMarble, programTexture, programWood, programDepth, programSkybox, programMirror;
	static SGE::Model ground, marble, box;
	static glm::vec3 lightPosition;

	static void Render(bool depth, SGE::Camera &camera) {

		programSkybox.setVec3f("cameraPosition", camera.getPosition());
		programSkybox.setMat4f("transformCameraMatrix", camera.getTransformMatrix());
		programBricks.setMat4f("transformCameraMatrix", camera.getTransformMatrix());
		programBricks.setVec3f("cameraPosition", camera.getPosition());
		programMirror.setMat4f("transformCameraMatrix", camera.getTransformMatrix());
		programMirror.setVec3f("cameraPosition", camera.getPosition());
		programTexture.setMat4f("transformCameraMatrix", camera.getTransformMatrix());
//		programTexture.setVec3f("cameraPosition", camera.getPosition());
		programWood.setMat4f("transformCameraMatrix", camera.getTransformMatrix());
		programWood.setVec3f("cameraPosition", camera.getPosition());

		SGE::Affine affineTransform;
		affineTransform.reset();
		affineTransform.setRotation(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));

		// Skybox background first
		if(!depth) {
			glCullFace(GL_FRONT);
			glDisable(GL_DEPTH_TEST);
			programSkybox.bind();
			box.draw();
			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_BACK);
		}

		affineTransform.reset();
		affineTransform.setScale(glm::vec3(2.0f));
		affineTransform.setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
		//affineTransform.setRotation(glm::vec3(0.5f * window.getTimeSeconds(), 0.0f, 0.0f));
		if(!depth) {
			programBricks.setMat4f("transformAffineMatrix", affineTransform.getTransformMatrix());
			programBricks.setMat3f("transformNormalMatrix", affineTransform.getNormalMatrix());
		} else {
			programDepth.setMat4f("transformAffineMatrix", affineTransform.getTransformMatrix());
		}
		ground.draw();

		affineTransform.reset();
		affineTransform.setScale(glm::vec3(0.25f));
		affineTransform.setPosition(glm::vec3(0.0f, 0.2f, 0.0f));
		if(!depth) {
			programBricks.setMat4f("transformAffineMatrix", affineTransform.getTransformMatrix());
			programBricks.setMat3f("transformNormalMatrix", affineTransform.getNormalMatrix());
		} else {
			programDepth.setMat4f("transformAffineMatrix", affineTransform.getTransformMatrix());
		}
		box.draw();

		affineTransform.reset();
		affineTransform.setScale(glm::vec3(0.25f));
		affineTransform.setPosition(glm::vec3(0.5f, 0.25f, 0.5f));
		if(!depth) {
			programMirror.setMat4f("transformAffineMatrix", affineTransform.getTransformMatrix());
			programMirror.setMat3f("transformNormalMatrix", affineTransform.getNormalMatrix());
		} else {
			programDepth.setMat4f("transformAffineMatrix", affineTransform.getTransformMatrix());
		}
		marble.draw();

		affineTransform.reset();
		affineTransform.setScale(glm::vec3(0.05f));
		affineTransform.setPosition(glm::vec3(lightPosition));
		if(!depth) {
			programWood.setMat4f("transformAffineMatrix", affineTransform.getTransformMatrix());
			programWood.setMat3f("transformNormalMatrix", affineTransform.getNormalMatrix());
		} else {
			programDepth.setMat4f("transformAffineMatrix", affineTransform.getTransformMatrix());
		}
		marble.draw();
	}

	static void cubeBall() {
		GL::Window window;
		GL::Texture textureWood, textureSkybox;
		GL::Framebuffer frameDepthmap, cubemapEnviroment;

		window.create(640, 480, "", 4);

		SGE::Camera cameraMain(window.getRatio(), glm::vec3(0.0f, 3.0f, 4.0f));
		SGE::Camera cameraLight(1.0f, glm::vec3(0.0f, 3.0f, 4.0f));

		textureWood.createFromFile(CONFIG_ASSETS_PATH"images/textures/TexturesCom_Cargo0097/TexturesCom_Cargo0097_M.jpg");
		const char *cubemapFiles[] = {
				CONFIG_ASSETS_PATH"images/skybox/right.jpg", CONFIG_ASSETS_PATH"images/skybox/left.jpg",
				CONFIG_ASSETS_PATH"images/skybox/top.jpg", CONFIG_ASSETS_PATH"images/skybox/bottom.jpg",
				CONFIG_ASSETS_PATH"images/skybox/front.jpg", CONFIG_ASSETS_PATH"images/skybox/back.jpg"
		};
		textureSkybox.createCubemapFromFiles(cubemapFiles);

		programBricks.createFromFile(CONFIG_ASSETS_PATH"programs/demo/shader.vert", CONFIG_ASSETS_PATH"programs/demo/shader_bricks.frag");
		programMarble.createFromFile(CONFIG_ASSETS_PATH"programs/demo/shader.vert", CONFIG_ASSETS_PATH"programs/demo/shader_marble.frag");
		programTexture.createFromFile(CONFIG_ASSETS_PATH"programs/demo/shader.vert", CONFIG_ASSETS_PATH"programs/demo/shader_texture.frag");
		programWood.createFromFile(CONFIG_ASSETS_PATH"programs/demo/shader.vert", CONFIG_ASSETS_PATH"programs/demo/shader_wood.frag");
		programDepth.createFromFile(CONFIG_ASSETS_PATH"programs/demo/depth.vert", CONFIG_ASSETS_PATH"programs/demo/depth.frag");
		programSkybox.createFromFile(CONFIG_ASSETS_PATH"programs/demo/skybox.vert", CONFIG_ASSETS_PATH"programs/demo/skybox.frag");
		programMirror.createFromFile(CONFIG_ASSETS_PATH"programs/demo/shader.vert", CONFIG_ASSETS_PATH"programs/demo/shader_mirror.frag");

		ground.createCube();
		marble.createSphere(32, 32);
		box.createCube();

		frameDepthmap.createFramebufferDepthmap();
		frameDepthmap.bindTextureDepth(1);

		cubemapEnviroment.createFramebufferCubemap();
		cubemapEnviroment.bindTextureDepth(7);
		programMirror.setInt("cubemapEnviroment", 7);

		// Bind textures to specific layers
		textureSkybox.bind(5);
		programSkybox.setInt("cubemapSkybox", 5);
//		programBricks.setInt("cubemapSkybox", 5);
//		programMirror.setInt("cubemapSkybox", 5);

		time_t t = time(nullptr) + time_delay;
		while(!window.shouldExit() && time(nullptr) < t) {
			window.setTitleFPS("");

			/// Set uniforms

			cameraMain.setAspectRatio(window.getRatio());

			lightPosition = glm::vec3(-0.5f + 1.0f * sinf(window.getTimeSeconds()), 1.2f, -1.5f + 1.0f * cosf(window.getTimeSeconds()));

			cameraMain.setFov(50.0f + 8.0f * cosf(window.getTimeSeconds() * 0.5f));
			cameraMain.setPosition(glm::vec3(4.5f * cosf(window.getTimeSeconds() * 0.2f), 1.0f, 4.5f * sinf(window.getTimeSeconds() * 0.2f)));

			programBricks.setMat4f("transformCameraMatrixDepth", cameraLight.getTransformMatrix());
			programBricks.setVec3f("lightPosition", lightPosition);
//			programBricks.setInt("textureAlbedo", 0);
			programBricks.setInt("textureDepthmap", 1);

			programMirror.setMat4f("transformCameraMatrixDepth", cameraLight.getTransformMatrix());
			programMirror.setVec3f("lightPosition", lightPosition);
//			programMirror.setInt("textureAlbedo", 0);
			programMirror.setInt("textureDepthmap", 1);

//			programTexture.setMat4f("transformCameraMatrixDepth", cameraLight.getTransformMatrix());
//			programTexture.setVec3f("lightPosition", lightPosition);
			programTexture.setInt("textureAlbedo", 0);
//			programTexture.setInt("textureDepthmap", 1);

//			programWood.setMat4f("transformCameraMatrixDepth", cameraLight.getTransformMatrix());
			programWood.setVec3f("lightPosition", lightPosition);
//			programWood.setInt("textureAlbedo", 0);
//			programWood.setInt("textureDepthmap", 1);

			/// Render scene depth map

			frameDepthmap.setViewPort();
			glCullFace(GL_FRONT);

			cameraLight.setFov(90.0f);
			cameraLight.setPosition(lightPosition);

			programDepth.setMat4f("transformCameraMatrixDepth", cameraLight.getTransformMatrix());

			Render(true, cameraMain);

			/// Render scene to cubemapEnviroment

			SGE::Camera cameraObject(1.0f, glm::vec3(0.5f, 0.25f, 0.5f), glm::vec3(0.0), 90.0f);
			std::array<std::pair<glm::vec3, glm::vec3>, 6> array = {
					std::make_pair(glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.0,-1.0, 0.0)),
					std::make_pair(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0,-1.0, 0.0)),
					std::make_pair(glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0)),
					std::make_pair(glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0, 0.0,-1.0)),
					std::make_pair(glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0,-1.0, 0.0)),
					std::make_pair(glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0,-1.0, 0.0))
			};
			for(int i = 0; i < 6; ++i) {
				cubemapEnviroment.setViewPort(i);
				cameraObject.setLookAt(cameraObject.getPosition() + array[i].first);
				cameraObject.setUpVector(array[i].second);
				Render(false, cameraObject);
			}

			/// Render scene normally

			window.clear(glm::vec3(0.5f, 0.5f, 0.6f));
			window.setViewport();

			// Scene
			Render(false, cameraMain);

			// Debug
			/*SGE::AffineTransform affineTransform;
			affineTransform.reset();
			affineTransform.setPosition(glm::vec3(8.5f, 3.5f, 100.0f));
			cameraMain.setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
			cameraMain.setPosition(glm::vec3(0.0f, 0.0f, 118.0f));
			cameraMain.setFov(45.0f);
			programTexture.setMat4f("transformAffineMatrix", affineTransform.getTransformMatrix());
			programTexture.setMat3f("transformNormalMatrix", affineTransform.getNormalMatrix());
			programTexture.setMat4f("transformCameraMatrix", cameraMain.getTransformMatrix());
			frameDepthmap.bindTexture(0);
			box.draw();*/

			window.swapBuffer();
		}
	}

	void DemoStart() {
		for(uint32_t i = 0; i < 1; ++i) {
			model_normal();
			model_array();
			two3D();
			cubes();
			cubeBall();
		}
	}
}
