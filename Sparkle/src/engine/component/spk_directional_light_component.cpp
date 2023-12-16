#include "engine/component/spk_directional_light_component.hpp"

namespace spk
{
	DirectionalLight::GPUBuffer::GPUBuffer() : 
		direction(spk::Vector3(0, -1, 0)),
		intensity(0.25f),
		color(spk::Color(255, 255, 255))
	{

	}
	
	DirectionalLight::GPUBuffer::GPUBuffer(const spk::Vector3& p_direction, const float& p_intensity, const spk::Color& p_color) : 
		direction(p_direction),
		intensity(p_intensity),
		color(p_color)
	{

	}

    bool DirectionalLight::_onUpdate()
	{
		if (_lightUniformBlock == nullptr)
		{
			_lightUniformBlock = dynamic_pointer_cast<spk::Pipeline::UniformBlock>(spk::Pipeline::uniform(spk::Pipeline::Uniform::Key(0, 2)));
		}

		if (_needUpdate = true)
		{
			_lightUniformBlock->field(L"directionalLight") = _gpuBuffer;
			_needUpdate = false;
		}
		return (false);
	}

    void DirectionalLight::_onRender()
	{

	}

	DirectionalLight::DirectionalLight() :
		Component(L"DirectionLight"),
		_gpuBuffer(spk::Vector3(0, -1, 0), 0.25f, spk::Color(255, 255, 255)),
		_needUpdate(true)
	{

	}

	void DirectionalLight::setDirection(const spk::Vector3& p_direction)
	{
		_gpuBuffer.direction = p_direction;
		_needUpdate = true;
	}

	const spk::Vector3& DirectionalLight::direction() const
	{
		return (_gpuBuffer.direction);
	}
		
	void DirectionalLight::setIntensity(const float& p_intensity)
	{
		_gpuBuffer.intensity = p_intensity;
		_needUpdate = true;
	}
	
	const float& DirectionalLight::intensity() const
	{
		return (_gpuBuffer.intensity);
	}
	
	void DirectionalLight::setColor(const spk::Color& p_color)
	{
		_gpuBuffer.color = p_color;
		_needUpdate = true;
	}
	
	const spk::Color& DirectionalLight::color() const
	{
		return (_gpuBuffer.color);
	}
}