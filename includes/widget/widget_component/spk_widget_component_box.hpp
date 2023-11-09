#pragma once

#include "design_pattern/spk_value.hpp"
#include "graphics/spk_color.hpp"
#include "graphics/spk_area.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk::WidgetComponent
{
    class Box
    {
    private:
        static inline std::shared_ptr<spk::Pipeline> _renderingPipeline = nullptr;

        static const size_t BackgroundIndex = 0;
        static const size_t FrontgroundIndex = 1;
        std::shared_ptr<spk::Pipeline::Object> _renderingObjects[2];

        ValueWrapper<spk::Color> _frontgroundColor;
        ValueWrapper<spk::Color> _backgroundColor;
        ValueWrapper<size_t> _borderSize;
        ValueWrapper<float> _depth;
        ValueWrapper<spk::Area> _area;

        void _updateBackgroundColor();
        void _updateFrontgroundColor();
        void _updateDepth();

        void _updateFrontgroundVertices();
        void _updateBackgroundVertices();
        void _updateVertices();

    public:
        Box();

        void render();

        static inline ValueWrapper<spk::Color>::Default defaultFrontgroundColor = spk::Color(180, 180, 180, 255);
        static inline ValueWrapper<spk::Color>::Default defaultBackgroundColor = spk::Color(120, 120, 120, 255);
        static inline ValueWrapper<size_t>::Default defaultBorderSize = 1;
        static inline ValueWrapper<float>::Default defaultDepth = 1.0f;
        static inline ValueWrapper<spk::Area>::Default defaultArea = { spk::Vector2Int(0, 0), spk::Vector2UInt(100, 100) };

        Value<spk::Color>& frontgroundColor() { return (_frontgroundColor.value()); }
        Value<spk::Color>& backgroundColor() { return (_backgroundColor.value()); }
        Value<size_t>& borderSize() { return (_borderSize.value()); }
        Value<float>& depth() { return (_depth.value()); }
        Value<spk::Area>& area() { return (_area.value()); }

        const Value<spk::Color>& frontgroundColor() const { return (_frontgroundColor.value()); }
        const Value<spk::Color>& backgroundColor() const { return (_backgroundColor.value()); }
        const Value<size_t>& borderSize() const { return (_borderSize.value()); }
        const Value<float>& depth() const { return (_depth.value()); }
        const Value<spk::Area>& area() const { return (_area.value()); }
    };
}