#include "engine/component/spk_directional_light_component.hpp"

namespace spk
{
    bool DirectionalLight::_onUpdate()
	{
		if (_lightUniformBlock == nullptr)
		{
			_lightUniformBlock = dynamic_pointer_cast<spk::Pipeline::UniformBlock>(spk::Pipeline::uniform(spk::Pipeline::Uniform::Key(0, 2)));
		}

		if (_needUpdate = true)
		{
			_lightUniformBlock->field(L"directionalLight") = _direction;
			_needUpdate = false;
		}
		return (false);
	}

    void DirectionalLight::_onRender()
	{

	}

	DirectionalLight::DirectionalLight() :
		Component(L"DirectionLight"),
		_direction(spk::Vector3(0, -1, 0)),
		_needUpdate(true)
	{

	}

	void DirectionalLight::setDirection(const spk::Vector3& p_direction)
	{
		_direction = p_direction;
		_needUpdate = true;
	}

	const spk::Vector3& DirectionalLight::direction() const
	{
		return (_direction);
	}
}