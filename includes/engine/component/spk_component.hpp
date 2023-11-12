#pragma once

#include <memory>
#include "design_pattern/spk_activable_object.hpp"

namespace spk
{
	class GameObject;

	class Component : public spk::ActivableObject{
	protected:
		std::shared_ptr<GameObject> _owner;

		virtual bool _onUpdate() = 0;
		virtual void _onRender() = 0;

	public:
		Component(std::shared_ptr<GameObject> p_owner);

		std::shared_ptr<GameObject> owner() const;

		bool update();
		void render();
	};
}