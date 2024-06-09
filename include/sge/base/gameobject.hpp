
#ifndef SMOLGAMEENGINE_GAMEOBJECT_HPP
#define SMOLGAMEENGINE_GAMEOBJECT_HPP

#include "model.hpp"
#include "material.hpp"
#include "affine.hpp"

namespace SGE {
	class GameObject {
	public:
		std::shared_ptr<SGE::Model> model;
		std::shared_ptr<SGE::Material> material;
		SGE::Affine affine;
	};
}

#endif //SMOLGAMEENGINE_GAMEOBJECT_HPP
