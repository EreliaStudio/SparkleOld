#include "widget/widget_component/spk_widget_component_box.hpp"
#include "graphics/pipeline/spk_default_shader.hpp"
#include "graphics/spk_viewport.hpp"

namespace spk::WidgetComponent
{
	Box::Box():
		_frontgroundColor(std::shared_ptr<const ValueWrapper<spk::Color>::Default>(&defaultFrontgroundColor, [](const ValueWrapper<spk::Color>::Default* p_value){})),
		_backgroundColor(std::shared_ptr<const ValueWrapper<spk::Color>::Default>(&defaultBackgroundColor, [](const ValueWrapper<spk::Color>::Default* p_value){})),
		_borderSize(std::shared_ptr<const ValueWrapper<size_t>::Default>(&defaultBorderSize, [](const ValueWrapper<size_t>::Default* p_value){})),
    	_area(std::shared_ptr<const ValueWrapper<spk::Area>::Default>(&defaultArea, [](const ValueWrapper<spk::Area>::Default* p_value){})),
    	_depth(std::shared_ptr<const ValueWrapper<float>::Default>(&defaultDepth, [](const ValueWrapper<float>::Default* p_value){}))
	{
		if (_renderingPipeline == nullptr)
		{
			_renderingPipeline = std::make_shared<spk::Pipeline>(boxComponentVertexShaderModule, boxComponentFragmentShaderModule);
		}

		_renderingObjects[BackgroundIndex] = _renderingPipeline->createObject();
		_renderingObjects[FrontgroundIndex] = _renderingPipeline->createObject();
	}

	void Box::_updateBackgroundColor()
	{
		_renderingObjects[BackgroundIndex]->pushConstants(L"color") = _backgroundColor.get();
		_backgroundColor.resetUpdateFlag();
	}
	
	void Box::_updateFrontgroundColor()
	{
		_renderingObjects[FrontgroundIndex]->pushConstants(L"color") = _frontgroundColor.get();
		_frontgroundColor.resetUpdateFlag();
	}

	void Box::_updateDepth()
	{
		_renderingObjects[FrontgroundIndex]->pushConstants(L"depth") = spk::Viewport::convertDepth(_depth.get() + 0.1f);
		_renderingObjects[BackgroundIndex]->pushConstants(L"depth") = spk::Viewport::convertDepth(_depth.get());
		_depth.resetUpdateFlag();
	}
    
	void Box::_updateBackgroundVertices()
	{
		_renderingObjects[BackgroundIndex]->storage().vertices().clear();
		_renderingObjects[BackgroundIndex]->storage().indexes().clear();

		spk::Vector2Int anchor = _area.get().anchor();
		spk::Vector2UInt size = _area.get().size();

		spk::Vector2UInt offsets[4] = {
			spk::Vector2UInt(0, 0),
			spk::Vector2UInt(0, 1),
			spk::Vector2UInt(1, 0),
			spk::Vector2UInt(1, 1)
		};
		std::vector<spk::Vector2> points;

		for (size_t i = 0; i < 4; i++)
		{
			points.push_back(spk::Viewport::convertScreenToGPUCoordinates(anchor + size * offsets[i]));
		}

		std::vector<unsigned int> indexes = 
		{
			0, 1, 2, 2, 1, 3
		};

		_renderingObjects[BackgroundIndex]->storage().vertices() << points << std::endl;
		_renderingObjects[BackgroundIndex]->storage().indexes() << indexes << std::endl;
	}
	
	void Box::_updateFrontgroundVertices()
	{
		_renderingObjects[FrontgroundIndex]->storage().vertices().clear();
		_renderingObjects[FrontgroundIndex]->storage().indexes().clear();

		spk::Vector2Int anchor = _area.get().anchor() + spk::Vector2Int(_borderSize.get(), _borderSize.get());
		spk::Vector2UInt size = _area.get().size() - spk::Vector2UInt(2 * _borderSize.get(), 2 * _borderSize.get());

		spk::Vector2UInt offsets[4] = {
			spk::Vector2UInt(0, 0),
			spk::Vector2UInt(0, 1),
			spk::Vector2UInt(1, 0),
			spk::Vector2UInt(1, 1)
		};
		std::vector<spk::Vector2> points;

		for (size_t i = 0; i < 4; i++)
		{
			points.push_back(spk::Viewport::convertScreenToGPUCoordinates(anchor + size * offsets[i]));
		}
		
		std::vector<unsigned int> indexes = {
			0, 1, 2, 2, 1, 3
		};

		_renderingObjects[FrontgroundIndex]->storage().vertices() << points << std::endl;
		_renderingObjects[FrontgroundIndex]->storage().indexes() << indexes << std::endl;
	}

	void Box::_updateVertices()
	{
		_updateFrontgroundVertices();
		_updateBackgroundVertices();

		_borderSize.resetUpdateFlag();
		_area.resetUpdateFlag();
	}

	void Box::render()
	{
        if (_backgroundColor.needUpdate() == true)
			_updateBackgroundColor();
        if (_frontgroundColor.needUpdate() == true)
			_updateFrontgroundColor();
        if (_depth.needUpdate() == true)
			_updateDepth();
		if (_borderSize.needUpdate() == true || _area.needUpdate() == true)
			_updateVertices();

		_renderingObjects[FrontgroundIndex]->render();
		_renderingObjects[BackgroundIndex]->render();
	}
}