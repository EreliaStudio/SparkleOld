#include "widget/widget_component/spk_widget_component_box.hpp"
#include "graphics/pipeline/spk_default_shader.hpp"

namespace spk::WidgetComponent
{
	Box::Box() :
		_area(_defaultArea),
		_depth(_defaultDepth),
		_borderSize(_defaultBorderSize),
		_backgroundColor(_defaultBackgroundColor),
		_borderColor(_defaultBorderColor)
	{
		if (_renderingPipeline == nullptr)
			_renderingPipeline = std::make_shared<spk::Pipeline>(boxComponentVertexShaderModule, boxComponentFragmentShaderModule);
			
		_renderingObjects[BackgroundIndex] = _renderingPipeline->createObject();
		_renderingObjects[BorderIndex] = _renderingPipeline->createObject();
	}

	void Box::setBackgroundColor(const spk::Color& p_backgroundColor)
	{
		_backgroundColor = p_backgroundColor;
	}
		
	void Box::setBorderColor(const spk::Color& p_borderColor)
	{
		_borderColor = p_borderColor;
	}

	void Box::setColors(const spk::Color& p_backgroundColor, const spk::Color& p_borderColor)
	{
		setBackgroundColor(p_backgroundColor);
		setBorderColor(p_borderColor);
	}

	void Box::setGeometry(const spk::Area& p_area)
	{
		_area = p_area;
	}
	
	void Box::setGeometry(const size_t& p_borderSize)
	{
		_borderSize = p_borderSize;
	}

	void Box::setGeometry(const spk::Area& p_area, const size_t& p_borderSize)
	{
		setGeometry(p_area);
		setGeometry(p_borderSize);
	}

	void Box::setDepth(const float& p_depth)
	{
		_depth = p_depth;
	}

	void Box::_composeBackgroundObjectGeometry()
	{
		_renderingObjects[BackgroundIndex]->storage().vertices().clear();
		_renderingObjects[BackgroundIndex]->storage().indexes().clear();

		spk::Vector2Int anchor = _area.value.value().anchor();
		spk::Vector2UInt size = _area.value.value().size();

		std::vector<spk::Vector2> points
		{
			spk::Vector2(-0.9, -0.9), // spk::convertScreenToGPUCoordinate(anchor + size * spk::Vector2UInt(0, 0)),
			spk::Vector2(0.9, -0.9), // spk::convertScreenToGPUCoordinate(anchor + size * spk::Vector2UInt(1, 0)),
			spk::Vector2(-0.9, 0.9), // spk::convertScreenToGPUCoordinate(anchor + size * spk::Vector2UInt(0, 1)),
			spk::Vector2(0.9, 0.9), // spk::convertScreenToGPUCoordinate(anchor + size * spk::Vector2UInt(1, 1)),
		};
		
		std::vector<unsigned int> indexes = 
		{
			0, 1, 2, 2, 1, 3
		};

		_renderingObjects[BackgroundIndex]->storage().vertices() << points << std::endl;
		_renderingObjects[BackgroundIndex]->storage().indexes() << indexes << std::endl;
	}
	
	void Box::_composeBorderObjectGeometry()
	{
		spk::Vector2Int anchor = _area.value.value().anchor();
		spk::Vector2UInt size = _area.value.value().size();

		std::vector<spk::Vector2> points
		{
			spk::Vector2(-1, -1), // spk::convertScreenToGPUCoordinate(anchor + size * spk::Vector2UInt(0, 0)),
			spk::Vector2(1, -1), // spk::convertScreenToGPUCoordinate(anchor + size * spk::Vector2UInt(1, 0)),
			spk::Vector2(-1, 1), // spk::convertScreenToGPUCoordinate(anchor + size * spk::Vector2UInt(0, 1)),
			spk::Vector2(1, 1), // spk::convertScreenToGPUCoordinate(anchor + size * spk::Vector2UInt(1, 1)),
		};
		
		std::vector<unsigned int> indexes = 
		{
			0, 1, 2, 2, 1, 3
		};

		_renderingObjects[BorderIndex]->storage().vertices() << points << std::endl;
		_renderingObjects[BorderIndex]->storage().indexes() << indexes << std::endl;
	}

	
	void Box::_updateArea()
	{
		_composeBackgroundObjectGeometry();
		_composeBorderObjectGeometry();

		_area.needUpdate = false;
	}
	
	void Box::_updateDepth()
	{
		_renderingObjects[BackgroundIndex]->pushConstants(L"depth") = _depth.value.value() + 0.01f;
		_renderingObjects[BorderIndex]->pushConstants(L"depth") = _depth.value.value();

		_depth.needUpdate = false;
	}
	
	void Box::_updateBackgroundColor()
	{
		_renderingObjects[BackgroundIndex]->pushConstants(L"color") = _backgroundColor.value.value();
		_backgroundColor.needUpdate = false;
	}
	
	void Box::_updateBorderColor()
	{
		_renderingObjects[BorderIndex]->pushConstants(L"color") = _borderColor.value.value();
		_borderColor.needUpdate = false;
	}

	void Box::_updateBorderSize()
	{
		_composeBackgroundObjectGeometry();
		_borderSize.needUpdate = false;
	}

	void Box::render()
	{
		if (_area.needUpdate == true)
			_updateArea();

		if (_depth.needUpdate == true)
			_updateDepth();

		if (_backgroundColor.needUpdate == true)
			_updateBackgroundColor();

		if (_borderColor.needUpdate == true)
			_updateBorderColor();

		if (_borderSize.needUpdate == true)
			_updateBorderSize();

		_renderingObjects[BorderIndex]->render();
		_renderingObjects[BackgroundIndex]->render();
	}
}