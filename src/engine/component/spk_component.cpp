#include "engine/component/spk_component.hpp"

namespace spk
{
	Component::Component(std::shared_ptr<GameObject> p_owner) :
		_owner(p_owner)
	{

	}

	std::shared_ptr<GameObject> Component::owner() const
	{
		return (_owner);
	}

	bool Component::update()
	{
		if (isActive() == true)
		{
			return (_onUpdate());
		}
		return (false);
	}

	void Component::render()
	{
		if (isActive() == true)
		{
			_onRender();
		}
	}
}