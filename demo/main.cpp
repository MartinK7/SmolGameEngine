
#include <array>
#include <memory>
#include <iostream>
#include <fstream>
#include "sge/base/sge.hpp"
#include "demo.hpp"
#include "player.hpp"
#include "pointlight.hpp"
#include "background.hpp"

#define PATH "../demo/assets"

namespace Game {

	static glm::vec3 fromBlender(glm::vec3 vec) {
		return glm::vec3(vec.x, vec.z, -vec.y);
	}

	static GL::Program programBMaterial, programBright;

	static std::map<std::string, std::unique_ptr<SGE::GameObject>> gameObjects;
	static GL::Framebuffer teapotCubemap;

	static std::vector<glm::vec3> lightPath;

	static inline float map(float value, float min1, float max1, float min2, float max2) {
		return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
	}

	static void loadTextures() {
		/// Create textures
		teapotCubemap.createFramebufferCubemap();
		teapotCubemap.bindTextureColor(21);
	}

	static void loadModels() {

		const char *models_names[] = {"boxes", "bulb", "light_frame", "floor", "walls", "vent", "vent_pipe", "vent_hold", "vent_prop", "teapot"};
		for(auto &o : models_names) {
			// Load model data
			std::unique_ptr<SGE::GameObject> gameObject(new SGE::GameObject);
			gameObject->model = std::make_shared<SGE::Model>();
			gameObject->model->createFromOBJ(PATH"/models/test_scene/" + std::string(o) + ".obj");

			// Create material
			gameObject->bmaterial = std::make_shared<SGE::BMaterial>();

			// Create transformation
			gameObject->affine = std::make_shared<SGE::Affine>();

			gameObjects[o] = std::move(gameObject);
		}

		// Experimental, attach texture to one specific object
		if(gameObjects.contains("boxes")) {
			// Load texture
			std::shared_ptr<GL::Texture> texture = std::make_shared<GL::Texture>();
			texture->createFromFile(PATH"/images/textures/TexturesCom_Cargo0097/TexturesCom_Cargo0097_M.jpg");
			gameObjects["boxes"]->bmaterial->setTextureAlbedo(texture);
			// Texture only
			gameObjects["boxes"]->bmaterial->setMixColorTextureAlbedo(1.0f);
		}

		if(gameObjects.contains("vent")) {
			// Load texture
			std::shared_ptr<GL::Texture> texture = std::make_shared<GL::Texture>();
			texture->createFromFile(PATH"/images/textures/TexturesCom_MetalBare0212_7_M.jpg");
			gameObjects["vent"]->bmaterial->setTextureAlbedo(texture);
			// Texture only
			gameObjects["vent"]->bmaterial->setMixColorTextureAlbedo(1.0f);
		}

		if(gameObjects.contains("floor")) {
			// Load texture
			std::shared_ptr<GL::Texture> texture = std::make_shared<GL::Texture>();
			texture->createFromFile(PATH"/images/textures/TexturesCom_ConcreteBare0330_7_seamless_S.png");
			gameObjects["floor"]->bmaterial->setTextureAlbedo(texture);
			// Texture only
			gameObjects["floor"]->bmaterial->setMixColorTextureAlbedo(1.0f);
		}

		if(gameObjects.contains("walls")) {
			// Load texture
			std::shared_ptr<GL::Texture> textureAlbedo = std::make_shared<GL::Texture>();
			textureAlbedo->createFromFile(PATH"/images/textures/TexturesCom_Brick_Rustic2_1K/TexturesCom_Brick_Rustic2_1K_albedo.png");
			gameObjects["walls"]->bmaterial->setTextureAlbedo(textureAlbedo);
			// Load texture
			std::shared_ptr<GL::Texture> textureNormal = std::make_shared<GL::Texture>();
			textureNormal->createFromFile(PATH"/images/textures/TexturesCom_Brick_Rustic2_1K/TexturesCom_Brick_Rustic2_1K_normal.png");
			gameObjects["walls"]->bmaterial->setTextureNormal(textureNormal);
			// Texture only
			gameObjects["walls"]->bmaterial->setMixColorTextureAlbedo(1.0f);
		}

		if(gameObjects.contains("teapot")) {
			// Texture only
			gameObjects["teapot"]->bmaterial->setColorAlbedo(glm::vec3(1.0f, 0.8f, 0.8f));
			gameObjects["teapot"]->bmaterial->setGlossy(0.5);
			// Position
			gameObjects["teapot"]->affine->setPosition(fromBlender({-3.35234f, -0.57187f, 0.0f}));
			gameObjects["teapot"]->affine->setScale(glm::vec3(0.8f));
		}
	}

	static void loadPaths() {
		std::ifstream path;
		path.open(PATH"/paths/lightpath.txt");
		for(std::string line; std::getline(path, line); ) {
			glm::vec3 p;
			if(sscanf(line.c_str(), "%f %f %f", &p.x, &p.y, &p.z) == 3) {
				lightPath.push_back(p);
			}
		}
		path.close();
	}

	static void loadPrograms() {
		programBMaterial.createFromFile(PATH"/programs/bmaterial/bmaterial.vert", PATH"/programs/bmaterial/bmaterial.frag");
		programBright.createFromFile(PATH"/programs/bright/bright.vert", PATH"/programs/bright/bright.frag");
	}

	static void updateProgramsUniforms(bool once) {
		if(once) {
			// Default model matrices
			programBMaterial.setMat4f("matrixModel", glm::mat4(1.0f));
			programBright.setMat4f("matrixModel", glm::mat4(1.0f));

			// Bind uniform textures to layers
			programBMaterial.setInt("cubemapBackground", 20);
			programBMaterial.setInt("sge_pointLight.cubemapDepthmap", 10);
			programBMaterial.setInt("sge_material.cubemapEnvironment", 21);
		}


		// Lights
		programBMaterial.setVec3f("sge_pointLight.color", glm::vec3(0.7f, 0.7f, 0.7f));
		programBMaterial.setFloat("sge_pointLight.intensity", 1.5f);
	}

	static void Start() {
		GL::Window window;
		window.create(640 * 2.5, 480 * 2);

		loadPrograms();
		loadModels();
		loadTextures();
		loadPaths();

		const char *cubemapBackgroundFiles[6] = {
				PATH"/images/cubemaps/Maskonaive2/posx.jpg", PATH"/images/cubemaps/Maskonaive2/negx.jpg",
				PATH"/images/cubemaps/Maskonaive2/posy.jpg", PATH"/images/cubemaps/Maskonaive2/negy.jpg",
				PATH"/images/cubemaps/Maskonaive2/posz.jpg", PATH"/images/cubemaps/Maskonaive2/negz.jpg"
		};
		Game::Background background(cubemapBackgroundFiles);

		std::shared_ptr<GL::Program> programBDepth;
		programBDepth = std::make_shared<GL::Program>();
		programBDepth->createFromFile(PATH"/programs/bdepth/bdepth.vert", PATH"/programs/bdepth/bdepth.frag");

		Game::PointLight pointLight(fromBlender({-5.42395f, -3.00282f, 3.03571f}), programBDepth);

		Game::Player player(fromBlender({-3.77249f, 6.23212f, 1.99697f}), glm::vec2(214.0f, -9.0));

		bool once = true;

		while(!window.shouldExit()) {
			window.setTitleFPS("");
			window.getMouseDelta();

			// Animate point light
			float t = window.getTimeSeconds();
			uint32_t index = (uint32_t)(t * 10.0f);
			float t0 = (float)(index + 0) / 10.0f;
			float t1 = (float)(index + 1) / 10.0f;
			if(index + 1 < lightPath.size()) {
				pointLight.setPosition(glm::mix(lightPath[index], lightPath[index + 1], map(t, t0, t1, 0.0f, 1.0f)));
			}

			// Player camera
			player.update(window);
			player.setUniforms(programBMaterial);
			player.setUniforms(programBright);

			pointLight.setUniforms(programBMaterial);
			pointLight.setUniforms(programBright);

			updateProgramsUniforms(once);

			pointLight.renderDepthCubemap([](std::shared_ptr<GL::Program> program) {
				//glCullFace(GL_FRONT);
				for (auto &o: gameObjects) {
					/*if(o.second->bmaterial) {
						o.second->bmaterial->setUniforms(*program);
					}*/
					if (o.second->affine) {
						o.second->affine->setUniforms(*program);
					}
					if (o.second->model) {
						o.second->model->draw();
					}
				}
				//glCullFace(GL_BACK);
			});

			#if 0
			/// Drawing - textures
			SGE::Camera cameraCubemap(1.0f, glm::vec3(0.0), glm::vec3(0.0), 90.0f);
			std::array<std::pair<glm::vec3, glm::vec3>, 6> cameraCubemapList = {
				std::make_pair(glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.0,-1.0, 0.0)),
				std::make_pair(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0,-1.0, 0.0)),
				std::make_pair(glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0)),
				std::make_pair(glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0, 0.0,-1.0)),
				std::make_pair(glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0,-1.0, 0.0)),
				std::make_pair(glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0,-1.0, 0.0))
			};
			// Render teapot environment cubemap
			if(gameObjects.contains("teapot")) {
				cameraCubemap.setPosition(gameObjects["teapot"]->affine->getPosition() + glm::vec3(0.0f, 0.1f, 0.0f));
				for(int i = 0; i < 6; ++i) {
					// Bind target cubemap face
					teapotCubemap.setViewPort(i);

					// Camera
					cameraCubemap.setLookAt(cameraCubemap.getPosition() + cameraCubemapList[i].first);
					cameraCubemap.setUpVector(cameraCubemapList[i].second);

					// Objects in scene
					programBMaterial.setMat4f("matrixCamera", cameraCubemap.getTransformMatrix());
					for(auto &o : gameObjects) {
						if(o.first == "teapot") {
							continue;
						}
						if(o.second->bmaterial) {
							o.second->bmaterial->setUniforms(programBMaterial);
						}
						if(o.second->affine) {
							o.second->affine->setUniforms(programBMaterial);
						}
						if(o.second->model) {
							o.second->model->draw();
						}
					}
					{
						SGE::BMaterial mat;
						mat.setColorAlbedo(glm::vec3(1.0f, 0.0f, 0.0f));
						SGE::Affine transform;
						transform.reset();
						transform.setScale(glm::vec3(0.1f));
						transform.setPosition(player.getPosition());
						programBMaterial.setMat4f("matrixModel", transform.getTransformMatrix());
						mat.setUniforms(programBMaterial);
						SGE::Model modelCube;
						modelCube.createCube();
						modelCube.draw();
					}
				}
			}
			#endif

			/// Drawing - main window
			window.setViewport();

			// Draw background first
			#if 1
			background.draw(window, player);
			#endif

			#if 1
			// Objects in scene
			player.setUniforms(programBMaterial);
			for(auto &o : gameObjects) {
				if(o.second->bmaterial) {
					o.second->bmaterial->setUniforms(programBMaterial);
				}
				if(o.second->affine) {
					o.second->affine->setUniforms(programBMaterial);
				}
				if(o.second->model) {
					o.second->model->draw();
				}
			}
			#endif

			#if 1
			// Debugging objects
			pointLight.renderDebug(programBright);
			#endif

			/// End of drawing

			window.swapBuffer();

			/// Update other post draw variables

			once = false;
		}
	}
}

int main() {
	//GL::Test::DemoStart();
	Game::Start();
	return 0;
}
