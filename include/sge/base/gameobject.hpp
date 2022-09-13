
#ifndef SMOLGAMEENGINE_GAMEOBJECT_HPP
#define SMOLGAMEENGINE_GAMEOBJECT_HPP

#include "model.hpp"
#include "bmaterial.hpp"
#include "affine.hpp"

namespace SGE {
	class GameObject {
	public:
		std::shared_ptr<SGE::Model> model;
		std::shared_ptr<SGE::BMaterial> bmaterial;
		std::shared_ptr<SGE::Affine> affine;
	};
}

#endif //SMOLGAMEENGINE_GAMEOBJECT_HPP
