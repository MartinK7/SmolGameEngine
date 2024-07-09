
#include <array>
#include <memory>
#include <iostream>
#include <fstream>
#include "sge/base/sge.hpp"
#include "demo.hpp"
#include "player.hpp"
#include "pointlight.hpp"
#include "background.hpp"

namespace Game {

	static glm::vec3 FromBlender(glm::vec3 vec) {
		return glm::vec3(vec.x, vec.z, -vec.y);
	}

	static inline float Map(float value, float min1, float max1, float min2, float max2) {
		return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
	}

	static void LoadModels(std::map<std::string, std::unique_ptr<SGE::GameObject>> &gameObjects) {
		const char *models_names[] = {"boxes", "bulb", "light_frame", "floor", "walls", "vent", "vent_pipe", "vent_hold", "vent_prop", "teapot"};
		for(auto &o : models_names) {
			// Load model data
			std::unique_ptr<SGE::GameObject> gameObject(new SGE::GameObject);
			gameObject->model = std::make_shared<SGE::Model>();
			gameObject->model->createFromOBJ(CONFIG_ASSETS_PATH"/models/test_scene/" + std::string(o) + ".obj");

			// Create material
			gameObject->material = std::make_shared<SGE::Material>();

			gameObjects[o] = std::move(gameObject);
		}

		// Experimental, attach texture to one specific object
		if(gameObjects.contains("boxes")) {
			// Load texture
			std::shared_ptr<GL::Texture> texture = std::make_shared<GL::Texture>();
			texture->createFromFile(CONFIG_ASSETS_PATH"/images/textures/TexturesCom_Cargo0097/TexturesCom_Cargo0097_M.jpg");
			gameObjects["boxes"]->material->setTextureAlbedo(texture);
			// Texture only
			gameObjects["boxes"]->material->setMixColorTextureAlbedo(1.0f);
		}

		if(gameObjects.contains("vent")) {
			// Load texture
			std::shared_ptr<GL::Texture> texture = std::make_shared<GL::Texture>();
			texture->createFromFile(CONFIG_ASSETS_PATH"/images/textures/TexturesCom_MetalBare0212_7_M.jpg");
			gameObjects["vent"]->material->setTextureAlbedo(texture);
			// Texture only
			gameObjects["vent"]->material->setMixColorTextureAlbedo(1.0f);
		}

		if(gameObjects.contains("floor")) {
			// Load texture
			std::shared_ptr<GL::Texture> texture = std::make_shared<GL::Texture>();
			texture->createFromFile(CONFIG_ASSETS_PATH"/images/textures/TexturesCom_ConcreteBare0330_7_seamless_S.png");
			gameObjects["floor"]->material->setTextureAlbedo(texture);
			// Texture only
			gameObjects["floor"]->material->setMixColorTextureAlbedo(1.0f);
		}

		if(gameObjects.contains("walls")) {
			// Load texture
			std::shared_ptr<GL::Texture> textureAlbedo = std::make_shared<GL::Texture>();
			textureAlbedo->createFromFile(CONFIG_ASSETS_PATH"/images/textures/TexturesCom_Brick_Rustic2_1K/TexturesCom_Brick_Rustic2_1K_albedo.png");
			gameObjects["walls"]->material->setTextureAlbedo(textureAlbedo);
			// Load texture
			std::shared_ptr<GL::Texture> textureNormal = std::make_shared<GL::Texture>();
			textureNormal->createFromFile(CONFIG_ASSETS_PATH"/images/textures/TexturesCom_Brick_Rustic2_1K/TexturesCom_Brick_Rustic2_1K_normal.png");
			gameObjects["walls"]->material->setTextureNormal(textureNormal);
			// Texture only
			gameObjects["walls"]->material->setMixColorTextureAlbedo(1.0f);
		}

		if(gameObjects.contains("teapot")) {
			// Texture only
			gameObjects["teapot"]->material->setColorAlbedo(glm::vec3(1.0f, 0.8f, 0.8f));
			gameObjects["teapot"]->material->setGlossy(0.5);
			// Position
			gameObjects["teapot"]->affine.setPosition(FromBlender({-3.35234f, -0.57187f, 0.0f}));
			gameObjects["teapot"]->affine.setScale(glm::vec3(0.8f));
		}
	}

	static void LoadPath(std::vector<glm::vec3> &path) {
		std::ifstream pathStream;
		pathStream.open(CONFIG_ASSETS_PATH"/paths/lightpath.txt");
		for(std::string line; std::getline(pathStream, line); ) {
			glm::vec3 p;
			if(sscanf(line.c_str(), "%f %f %f", &p.x, &p.y, &p.z) == 3) {
				path.push_back(p);
			}
		}
		pathStream.close();
	}

	static void LoadPrograms(std::map<std::string, std::unique_ptr<GL::Program>> &programs) {
		auto program = std::make_unique<GL::Program>();
		program->createFromFile(CONFIG_ASSETS_PATH"programs/material/material.vert", CONFIG_ASSETS_PATH"programs/material/material.frag");
		programs["programMaterial"] = std::move(program);

		program = std::make_unique<GL::Program>();
		program->createFromFile(CONFIG_ASSETS_PATH"programs/bright/bright.vert", CONFIG_ASSETS_PATH"programs/bright/bright.frag");
		programs["programBright"] = std::move(program);
	}

	static void UpdateProgramsUniforms(bool once, std::map<std::string, std::unique_ptr<GL::Program>> &programs) {
		if(once) {
			// Default model matrices
			programs["programMaterial"]->setMat4f("matrixModel", glm::mat4(1.0f));
			programs["programBright"]->setMat4f("matrixModel", glm::mat4(1.0f));

			// Bind uniform textures to layers
			programs["programMaterial"]->setInt("cubemapBackground", 20);
			programs["programMaterial"]->setInt("sge_pointLight.cubemapDepthmap", 10);
			programs["programMaterial"]->setInt("sge_material.cubemapEnvironment", 21);
		}

		// Lights
		programs["programMaterial"]->setVec3f("sge_pointLight.color", glm::vec3(0.7f, 0.7f, 0.7f));
		programs["programMaterial"]->setFloat("sge_pointLight.intensity", 1.5f);
	}

	static void RenderScene(std::map<std::string, std::unique_ptr<SGE::GameObject>> &gameObjects, GL::Program &targetProgram, bool setMaterial = true, const std::list<std::string> &except = {})
	{
		for(auto &o : gameObjects) {
			bool skip = false;
			for(auto &e : except) {
				if(o.first == e) {
					skip = true;
					break;
				}
			}
			if(skip) {
				continue;
			}

			if(setMaterial && o.second->material) {
				o.second->material->setUniforms(targetProgram);
			}
			o.second->affine.setUniforms(targetProgram);
			if(o.second->model) {
				o.second->model->draw();
			}
		}
	}

	static void Start() {
		GL::Window window;
		window.create(640 * 2, 480 * 2);

		std::map<std::string, std::unique_ptr<SGE::GameObject>> gameObjects;
		std::map<std::string, std::unique_ptr<GL::Program>> programs;

		std::vector<glm::vec3> lightPath;

		/// Create textures
		GL::Framebuffer teapotCubemap;
		teapotCubemap.createFramebufferCubemap();
		teapotCubemap.bindTextureColor(21);

		LoadPrograms(programs);
		LoadModels(gameObjects);

		LoadPath(lightPath);

		const char *cubemapBackgroundFiles[6] = {
				CONFIG_ASSETS_PATH"/images/cubemaps/Maskonaive2/posx.jpg", CONFIG_ASSETS_PATH"/images/cubemaps/Maskonaive2/negx.jpg",
				CONFIG_ASSETS_PATH"/images/cubemaps/Maskonaive2/posy.jpg", CONFIG_ASSETS_PATH"/images/cubemaps/Maskonaive2/negy.jpg",
				CONFIG_ASSETS_PATH"/images/cubemaps/Maskonaive2/posz.jpg", CONFIG_ASSETS_PATH"/images/cubemaps/Maskonaive2/negz.jpg"
		};
		Game::Background background(cubemapBackgroundFiles);

		std::shared_ptr<GL::Program> programDepth;
		programDepth = std::make_shared<GL::Program>();
		programDepth->createFromFile(CONFIG_ASSETS_PATH"programs/demo/depth.vert", CONFIG_ASSETS_PATH"programs/demo/depth.frag");

		Game::PointLight pointLight(FromBlender({-5.42395f, -3.00282f, 3.03571f}), programDepth);

		Game::Player player(FromBlender({-3.77249f, 6.23212f, 1.99697f}), glm::vec2(214.0f, -9.0));

		bool once = true;

		while(!window.shouldExit()) {
			window.setTitleFPS("");
			window.getMouseDelta();

			// Animate point light
			float t = window.getTimeSeconds();//10.0;
			uint32_t index = (uint32_t)(t * 10.0f);
			float t0 = (float)(index + 0) / 10.0f;
			float t1 = (float)(index + 1) / 10.0f;
			if(index + 1 < lightPath.size()) {
				pointLight.setPosition(glm::mix(lightPath[index], lightPath[index + 1], Map(t, t0, t1, 0.0f, 1.0f)));
			}

			// Player camera
			player.update(window);
			player.setUniforms(*programs["programMaterial"]);
			player.setUniforms(*programs["programBright"]);

			pointLight.setUniforms(*programs["programMaterial"]);
			pointLight.setUniforms(*programs["programBright"]);

			UpdateProgramsUniforms(once, programs);

			pointLight.renderDepthCubemap([&](std::shared_ptr<GL::Program> programPointLight) {
				//glCullFace(GL_FRONT);
				//glClearColor(1.0, 1.0, 1.0, 1.0);
				//glClear(GL_COLOR_BUFFER_BIT);
				RenderScene(gameObjects, *programPointLight, false, {""});
				//glCullFace(GL_BACK);
			});

			#if 1
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
				cameraCubemap.setPosition(gameObjects["teapot"]->affine.getPosition() + glm::vec3(0.0f, 0.1f, 0.0f));
				for(int i = 0; i < 6; ++i) {
					// Bind target cubemap face
					teapotCubemap.setViewPort(i);

					// Camera
					cameraCubemap.setLookAt(cameraCubemap.getPosition() + cameraCubemapList[i].first);
					cameraCubemap.setUpVector(cameraCubemapList[i].second);

					// Objects in scene
					programs["programMaterial"]->setMat4f("matrixCamera", cameraCubemap.getTransformMatrix());
					RenderScene(gameObjects, *programs["programMaterial"], true, {"teapot"});
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
			player.setUniforms(*programs["programMaterial"]);
			RenderScene(gameObjects, *programs["programMaterial"]);
			#endif

			#if 1
			// Debugging objects
			pointLight.renderDebug(*programs["programBright"]);
			#endif

			/// End of drawing

			window.swapBuffer();

			/// Update other post draw variables

			once = false;
		}
	}
}

int main() {
	// GL::Test::DemoStart();
	Game::Start();
	return 0;
}
