
#include "background.hpp"

Game::Background::Background(const char **cubemapBackgroundFiles) {
	modelCube.createCube();

	programBackground.createFromFile(CONFIG_ASSETS_PATH"programs/background/background.vert", CONFIG_ASSETS_PATH"programs/background/background.frag");
	programBackground.setInt("cubemapBackground", 10);

	cubemapBackground.createCubemapFromFiles(cubemapBackgroundFiles);
	cubemapBackground.bind(20);
}

void Game::Background::draw(GL::Window &window, Game::Player &player) {
	// Do not care about depth test and also invert face culling
	// (the used model cube normals are directed outward we need them inwards)
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);

	// Place temporary player camera inside cube and keep same rotation
	SGE::Camera cameraCubemap(window.getRatio(), glm::vec3(0.0f), player.getRotationVector());
	programBackground.setMat4f("matrixCamera", cameraCubemap.getTransformMatrix());

	// Draw simple cube which fills user view
	modelCube.draw();

	// Reset settings
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
}
