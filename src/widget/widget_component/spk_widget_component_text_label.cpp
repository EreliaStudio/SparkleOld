#include "widget/widget_component/spk_widget_component_text_label.hpp"
#include "graphics/pipeline/spk_default_shader.hpp"
#include "graphics/spk_viewport.hpp"

namespace spk::WidgetComponent
{
	TextLabel::TextLabel():
		_font(std::shared_ptr<const ValueWrapper<std::shared_ptr<Font>>::Default>(&defaultFont, [](const ValueWrapper<std::shared_ptr<Font>>::Default* p_value){})),
		_text(std::shared_ptr<const ValueWrapper<std::wstring>::Default>(&defaultText, [](const ValueWrapper<std::wstring>::Default* p_value){})),
		_verticalAlignment(std::shared_ptr<const ValueWrapper<VerticalAlignment>::Default>(&defaultVerticalAlignment, [](const ValueWrapper<VerticalAlignment>::Default* p_value){})),
		_horizontalAlignment(std::shared_ptr<const ValueWrapper<HorizontalAlignment>::Default>(&defaultHorizontalAlignment, [](const ValueWrapper<HorizontalAlignment>::Default* p_value){})),
		_textColor(std::shared_ptr<const ValueWrapper<spk::Color>::Default>(&defaultTextColor, [](const ValueWrapper<spk::Color>::Default* p_value){})),
		_textSize(std::shared_ptr<const ValueWrapper<size_t>::Default>(&defaultTextSize, [](const ValueWrapper<size_t>::Default* p_value){})),
		_outlineType(std::shared_ptr<const ValueWrapper<Font::OutlineType>::Default>(&defaultOutlineType, [](const ValueWrapper<Font::OutlineType>::Default* p_value){})),
		_outlineColor(std::shared_ptr<const ValueWrapper<spk::Color>::Default>(&defaultOutlineColor, [](const ValueWrapper<spk::Color>::Default* p_value){})),
		_outlineSize(std::shared_ptr<const ValueWrapper<size_t>::Default>(&defaultOutlineSize, [](const ValueWrapper<size_t>::Default* p_value){})),
		_anchor(std::shared_ptr<const ValueWrapper<spk::Vector2Int>::Default>(&defaultAnchor, [](const ValueWrapper<spk::Vector2Int>::Default* p_value){}))
	{
		if (_renderingPipeline == nullptr)
		{
			_renderingPipeline = std::make_shared<spk::Pipeline>(textLabelComponentVertexShaderModule, textLabelComponentFragmentShaderModule);
		}

		_renderingObject = _renderingPipeline->createObject();
	}

	void TextLabel::_updateVertices()
	{
		std::shared_ptr<spk::Font::Atlas> atlas = _font.get()->atlas(spk::Font::Key(_textSize.get(), _outlineSize.get(), _outlineType.get()));

		struct Unit
		{
			spk::Vector2 position;
			spk::Vector2 uvs;

			Unit(){}
			Unit(spk::Vector2 p_position, spk::Vector2 p_uvs) : position(p_position), uvs(p_uvs){}
		};

		std::vector<Unit> _units;

		for (size_t i = 0; i < _text.get().size(); i++)
		{
			const Font::Atlas::GlyphData& glyphData = atlas->glyph(_text.get()[i]);

			for (size_t j = 0; j < 4; j++)
			{
				_units.push_back(Unit(
						spk::Viewport::convertScreenToGPUCoordinates(_anchor.get() + glyphData.position[j]),
						glyphData.uvs[j]
					));
			}
		}

		_font.resetUpdateFlag();
		_text.resetUpdateFlag();
		_verticalAlignment.resetUpdateFlag();
		_horizontalAlignment.resetUpdateFlag();
		_textSize.resetUpdateFlag();
		_outlineType.resetUpdateFlag();
		_outlineSize.resetUpdateFlag();
		_anchor.resetUpdateFlag();
	}

	void TextLabel::_updateTextColor()
	{

		_textColor.resetUpdateFlag();
	}
	
	void TextLabel::_updateOutlineColor()
	{

		_outlineColor.resetUpdateFlag();
	}

	void TextLabel::render()
	{
		if (_font.needUpdate() == true ||
			_text.needUpdate() == true ||
			_verticalAlignment.needUpdate() == true ||
			_horizontalAlignment.needUpdate() == true ||
			_textSize.needUpdate() == true ||
			_outlineType.needUpdate() == true ||
			_outlineSize.needUpdate() == true ||
			_anchor.needUpdate() == true)
		{
			_updateVertices();
		}

        if (_textColor.needUpdate() == true)
		{
			_updateTextColor();
		}

        if (_outlineColor.needUpdate() == true)
		{
			_updateOutlineColor();
		}

		_renderingObject->render();
	}
}