#pragma once

#include <string>
#include "design_pattern/spk_value.hpp"
#include "graphics/spk_font.hpp"
#include "graphics/spk_color.hpp"

namespace spk::WidgetComponent
{
    class TextLabel
	{
	public:

    private:
        static inline std::shared_ptr<spk::Pipeline> _renderingPipeline = nullptr;
        std::shared_ptr<spk::Pipeline::Object> _renderingObject;

        ValueWrapper<std::shared_ptr<Font>> _font;
        std::shared_ptr<Font::Atlas> _fontAtlas;
        ValueWrapper<std::wstring> _text;
        ValueWrapper<Font::VerticalAlignment> _verticalAlignment;
        ValueWrapper<Font::HorizontalAlignment> _horizontalAlignment;
        ValueWrapper<spk::Color> _textColor;
        ValueWrapper<size_t> _textSize;
        ValueWrapper<Font::OutlineType> _outlineType;
        ValueWrapper<spk::Color> _outlineColor;
        ValueWrapper<size_t> _outlineSize;
        ValueWrapper<spk::Vector2Int> _anchor;
        ValueWrapper<float> _depth;

        struct RenderData
        {
            spk::Vector2Int size;
            spk::Vector2Int topLeftCorner;
            spk::Vector2Int downRightCorner;
            std::vector<const spk::Font::Atlas::GlyphData *> glyphDatas;
        };

        RenderData _computeRenderData();
        spk::Vector2Int _computeStringOffset(const RenderData& p_renderData);
        void _updateVertices();

        void _updateTextColor();
        void _updateOutlineColor();

        void _updateDepth();

    public:
        TextLabel();

        void render();

        static inline ValueWrapper<std::shared_ptr<Font>>::Default defaultFont = nullptr;
        static inline ValueWrapper<std::wstring>::Default defaultText = L"";
        static inline ValueWrapper<Font::VerticalAlignment>::Default defaultVerticalAlignment = Font::VerticalAlignment::Middle;
        static inline ValueWrapper<Font::HorizontalAlignment>::Default defaultHorizontalAlignment = Font::HorizontalAlignment::Middle;
        static inline ValueWrapper<spk::Color>::Default defaultTextColor = spk::Color(0, 0, 0, 255);
        static inline ValueWrapper<size_t>::Default defaultTextSize = 0;
        static inline ValueWrapper<Font::OutlineType>::Default defaultOutlineType = Font::OutlineType::Standard;
        static inline ValueWrapper<spk::Color>::Default defaultOutlineColor = spk::Color(0, 0, 0, 255);
        static inline ValueWrapper<size_t>::Default defaultOutlineSize = 0;
        static inline ValueWrapper<spk::Vector2Int>::Default defaultAnchor = spk::Vector2Int(0, 0);
        static inline ValueWrapper<float>::Default defaultDepth = 0.0f;

        Value<std::shared_ptr<Font>>& font() { return (_font.value()); }
        Value<std::wstring>& text() { return (_text.value()); }
        Value<Font::VerticalAlignment>& verticalAlignment() { return (_verticalAlignment.value()); }
        Value<Font::HorizontalAlignment>& horizontalAlignment() { return (_horizontalAlignment.value()); }
        Value<spk::Color>& textColor() { return (_textColor.value()); }
        Value<size_t>& textSize() { return (_textSize.value()); }
        Value<Font::OutlineType>& outlineType() { return (_outlineType.value()); }
        Value<spk::Color>& outlineColor() { return (_outlineColor.value()); }
        Value<size_t>& outlineSize() { return (_outlineSize.value()); }
        Value<spk::Vector2Int>& anchor() { return (_anchor.value()); }
        Value<float>& depth() { return (_depth.value()); }

        const Value<std::shared_ptr<Font>>& font() const { return (_font.value()); }
        const Value<std::wstring>& text() const { return (_text.value()); }
        const Value<Font::VerticalAlignment>& verticalAlignment() const { return (_verticalAlignment.value()); }
        const Value<Font::HorizontalAlignment>& horizontalAlignment() const { return (_horizontalAlignment.value()); }
        const Value<spk::Color>& textColor() const { return (_textColor.value()); }
        const Value<size_t>& textSize() const { return (_textSize.value()); }
        const Value<Font::OutlineType>& outlineType() const { return (_outlineType.value()); }
        const Value<spk::Color>& outlineColor() const { return (_outlineColor.value()); }
        const Value<size_t>& outlineSize() const { return (_outlineSize.value()); }
        const Value<spk::Vector2Int>& anchor() const { return (_anchor.value()); }
        const Value<float>& depth() const { return (_depth.value()); }
    };
}