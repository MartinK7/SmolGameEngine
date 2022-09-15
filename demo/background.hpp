
#ifndef SGE_DEMO_BACKGROUND_HPP
#define SGE_DEMO_BACKGROUND_HPP

#include <array>

#include "sge/base/camera.hpp"
#include "sge/gl/program.hpp"
#include "sge/gl/window.hpp"
#include "sge/base/model.hpp"

#include "player.hpp"

#define PATH "../demo/assets"

namespace Game {
	class Background {
		GL::Texture cubemapBackground;
		SGE::Model modelCube;
		GL::Program programBackground;
	public:
		Background(const char *cubemapBackgroundFiles[6]);
		void draw(GL::Window &window, Game::Player &player);
	};
}

#endif //SGE_DEMO_BACKGROUND_HPP
