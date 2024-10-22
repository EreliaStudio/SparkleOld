#include "engine/component/spk_component.hpp"
#include "engine/spk_game_object.hpp"
#include "application/spk_application.hpp"

namespace spk
{
	Component::Component(const std::wstring& p_name) :
		_owner(std::shared_ptr<spk::GameObject>(spk::GameObject::_insertingGameObject, [](spk::GameObject*){})),
		_name(p_name),
		_timeConsumptionMetrics(spk::Application::instance()->profiler().metrics<TimeConsumption>(p_name))
	{

	}

	Component::~Component()
	{
		
	}

	std::shared_ptr<GameObject> Component::owner()
	{
		return (_owner);
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
		MONITOR_TIME_CONSUMPTION(_timeConsumptionMetrics);
		return (_onUpdate());
	}

	void Component::render()
	{
		_onRender();
	}
}