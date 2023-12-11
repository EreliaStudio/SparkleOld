#include "widget/widget_component/spk_widget_component_text_label.hpp"
#include "graphics/spk_viewport.hpp"

namespace spk::WidgetComponent
{
    spk::ShaderModule TextLabel::VertexShaderModule = spk::ShaderModule(
		"TextLabelComponentVertexShader",
R"(#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUvs;

layout(push_constant) uniform PushConstant
{
	float depth;
	vec4 textColor;
	vec4 outlineColor;
} pushConstants;

layout(location = 0) out vec2 fragmentUVs;
layout(location = 1) out vec4 textColor;
layout(location = 2) out vec4 outlineColor;

void main() {
    gl_Position = vec4(inPosition, pushConstants.depth, 1.0);
	fragmentUVs = inUvs;
    textColor = pushConstants.textColor;
    outlineColor = pushConstants.outlineColor;
})"
	);

    spk::ShaderModule TextLabel::FragmentShaderModule = spk::ShaderModule(
		"TextLabelComponentFragmentShader",
R"(#version 450

layout(location = 0) in vec2 fragmentUVs;
layout(location = 1) in vec4 textColor;
layout(location = 2) in vec4 outlineColor;

layout(binding = 0) uniform sampler2D textureID;

layout(location = 0) out vec4 outColor;

void main() {
    float r = texture(textureID, fragmentUVs).r;

    if (r == 0.0) {
        discard;
    } else if (r == 1.0) {
        outColor = textColor;
    } else {
        outColor = outlineColor;
    }
})"
	);

	TextLabel::TextLabel() :
		_font(std::shared_ptr<const ValueWrapper<std::shared_ptr<Font>>::Default>(&defaultFont, [](const ValueWrapper<std::shared_ptr<Font>>::Default* p_value){})),
		_fontAtlas(nullptr),
		_text(std::shared_ptr<const ValueWrapper<std::wstring>::Default>(&defaultText, [](const ValueWrapper<std::wstring>::Default* p_value){})),
		_verticalAlignment(std::shared_ptr<const ValueWrapper<Font::VerticalAlignment>::Default>(&defaultVerticalAlignment, [](const ValueWrapper<Font::VerticalAlignment>::Default* p_value){})),
		_horizontalAlignment(std::shared_ptr<const ValueWrapper<Font::HorizontalAlignment>::Default>(&defaultHorizontalAlignment, [](const ValueWrapper<Font::HorizontalAlignment>::Default* p_value){})),
		_textColor(std::shared_ptr<const ValueWrapper<spk::Color>::Default>(&defaultTextColor, [](const ValueWrapper<spk::Color>::Default* p_value){})),
		_textSize(std::shared_ptr<const ValueWrapper<size_t>::Default>(&defaultTextSize, [](const ValueWrapper<size_t>::Default* p_value){})),
		_outlineType(std::shared_ptr<const ValueWrapper<Font::OutlineType>::Default>(&defaultOutlineType, [](const ValueWrapper<Font::OutlineType>::Default* p_value){})),
		_outlineColor(std::shared_ptr<const ValueWrapper<spk::Color>::Default>(&defaultOutlineColor, [](const ValueWrapper<spk::Color>::Default* p_value){})),
		_outlineSize(std::shared_ptr<const ValueWrapper<size_t>::Default>(&defaultOutlineSize, [](const ValueWrapper<size_t>::Default* p_value){})),
		_anchor(std::shared_ptr<const ValueWrapper<spk::Vector2Int>::Default>(&defaultAnchor, [](const ValueWrapper<spk::Vector2Int>::Default* p_value){})),
		_depth(std::shared_ptr<const ValueWrapper<float>::Default>(&defaultDepth, [](const ValueWrapper<float>::Default* p_value){}))
	{
		if (_renderingPipeline == nullptr)
		{
			_renderingPipeline = std::make_shared<spk::Pipeline>(TextLabel::VertexShaderModule, TextLabel::FragmentShaderModule);
		}

		_renderingObject = _renderingPipeline->createObject();
		_renderingPipelineTextureIDUniform = dynamic_pointer_cast<spk::Pipeline::SamplerUniform>(_renderingPipeline->uniform(L"textureID"));

		_renderingPipelineTextureIDUniform->set(0);
	}
	
    TextLabel::RenderData TextLabel::_computeRenderData()
	{
		RenderData result;

		result.topLeftCorner = 0;

		spk::Vector2Int stringOffset = 0;
		for (size_t i = 0; i < _text.get().size(); i++)
		{
			const Font::Atlas::GlyphData& glyphData = _fontAtlas->glyph(_text.get()[i]);
			
			if (result.topLeftCorner.y > glyphData.position[0].y)
				result.topLeftCorner.y = glyphData.position[0].y;

			result.glyphDatas.push_back(&glyphData);

			if (i < _text.get().size() - 1)
				stringOffset += glyphData.step;
		}

		result.downRightCorner = stringOffset;

		result.size = result.downRightCorner - result.topLeftCorner;

		return (result);
	}

	spk::Vector2Int TextLabel::_computeStringOffset(const TextLabel::RenderData& p_renderData)
	{
		spk::Vector2Int result = 0;

		switch (_horizontalAlignment.get())
		{
			case Font::HorizontalAlignment::Left:
			{
				result.x = 0;
				break;
			}
			
			case Font::HorizontalAlignment::Middle:
			{
				result.x = -p_renderData.size.x / 2;
				break;
			}
			
			case Font::HorizontalAlignment::Right:
			default:
			{
				result.x = -p_renderData.size.x;
				break;
			}
		}

		switch (_verticalAlignment.get())
		{
			case Font::VerticalAlignment::Top:
			{
				result.y = p_renderData.size.y;
				break;
			}
			
			case Font::VerticalAlignment::Middle:
			{
				result.y = p_renderData.size.y / 2;
				break;
			}
			
			case Font::VerticalAlignment::Down:
			default:
			{
				result.y = 0;
				break;
			}
		}

		return (result);
	}

	void TextLabel::_updateVertices()
	{
		_fontAtlas = _font.get()->atlas(spk::Font::Key(_textSize.get(), _outlineSize.get(), _outlineType.get()));

		struct Unit
		{
			spk::Vector2 position;
			spk::Vector2 uvs;

			Unit(){}
			Unit(spk::Vector2 p_position, spk::Vector2 p_uvs) : position(p_position), uvs(p_uvs){}
		};

		RenderData renderData = _computeRenderData();

		std::vector<Unit> units;
		std::vector<unsigned int> indexes;

		static const unsigned int indexesValues[6] = {0, 2, 3, 3, 1, 0};

		spk::Vector2Int stringOffset = _computeStringOffset(renderData);
		for (size_t i = 0; i < renderData.glyphDatas.size(); i++)
		{
			size_t baseIndexesValue = units.size();
			for (size_t j = 0; j < 4; j++)
			{
				units.push_back(Unit(
						spk::Viewport::convertScreenToGPUCoordinates(_anchor.get() + stringOffset + renderData.glyphDatas[i]->position[j]),
						renderData.glyphDatas[i]->uvs[j]
					));
			}
			stringOffset += renderData.glyphDatas[i]->step;

			for (size_t j = 0; j < 6; j++)
			{
				indexes.push_back(indexesValues[j] + baseIndexesValue);
			}
		}

		_renderingObject->storage().vertices() << units << std::endl;
		_renderingObject->storage().indexes() << indexes << std::endl;

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
		_renderingObject->pushConstants(L"textColor") = _textColor.get();
		_textColor.resetUpdateFlag();
	}
	
	void TextLabel::_updateOutlineColor()
	{
		_renderingObject->pushConstants(L"outlineColor") = _outlineColor.get();
		_outlineColor.resetUpdateFlag();
	}

	void TextLabel::_updateDepth()
	{
		_renderingObject->pushConstants(L"depth") = spk::Viewport::convertDepth(_depth.get());
		_depth.resetUpdateFlag();
	}

	void TextLabel::render()
	{
		if (_font.get() == nullptr)
			spk::throwException(L"Can't render a textLabel without font. Please setup default font and/or single ");

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

        if (_depth.needUpdate() == true)
		{
			_updateDepth();
		}

		if (_fontAtlas != nullptr)
		{
			_fontAtlas->texture().bind(0);
			_renderingPipelineTextureIDUniform->bind();

		}
		_renderingObject->render();

		if (_fontAtlas != nullptr)
		{
			_fontAtlas->texture().unbind();
		}
	}
}