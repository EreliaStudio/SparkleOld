#include "engine/component/spk_component.hpp"
#include "engine/spk_game_object.hpp"

namespace spk
{
	Component::Component(std::shared_ptr<GameObject> p_owner, const std::wstring& p_name) :
		_owner(p_owner),
		_name(p_name)
	{

	}

	std::shared_ptr<const GameObject> Component::owner() const
	{
		return (_owner);
	}

	const std::wstring& Component::name() const
	{
		return (_name);
	}

	std::wstring Component::fullname() const
	{
		return (_owner->name() + L"::" + _name);
	}

	bool Component::update()
	{
		return (_onUpdate());
	}

	void Component::render()
	{
		_onRender();
	}
}