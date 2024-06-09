
#ifndef SGE_DEMO_SCENE_HPP
#define SGE_DEMO_SCENE_HPP

#include <memory>
#include <map>

#include "sge/base/gameobject.hpp"
#include "sge/base/camera.hpp"

namespace SGE {
	class Scene {
		std::map<std::string, std::shared_ptr<SGE::Model>> models;
		std::map<std::string, std::shared_ptr<SGE::Material>> materials;
		std::map<std::string, std::unique_ptr<SGE::Camera>> cameras;

		// This connects models+material together
		std::map<std::string, std::unique_ptr<SGE::GameObject>> gameObjects;
	public:
		void loadFromFile(std::string &sceneFullPath);
	};
}

#endif //SGE_DEMO_SCENE_HPP
