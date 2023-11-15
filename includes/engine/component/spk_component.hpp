#pragma once

#include <memory>
#include "design_pattern/spk_activable_object.hpp"

namespace spk
{
	class GameObject;

	class Component{
	protected:
		std::shared_ptr<GameObject> _owner;
		std::wstring _name;

		virtual bool _onUpdate() = 0;
		virtual void _onRender() = 0;

	public:
		Component(std::shared_ptr<GameObject> p_owner, const std::wstring& p_name);

		std::shared_ptr<const GameObject> owner() const;
		const std::wstring& name() const;

		std::wstring fullname() const;

		bool update();
		void render();
	};
}