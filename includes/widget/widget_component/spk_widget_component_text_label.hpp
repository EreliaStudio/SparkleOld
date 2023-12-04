#pragma once

#include <string>
#include "design_pattern/spk_value.hpp"
#include "graphics/spk_font.hpp"
#include "graphics/spk_color.hpp"

namespace spk::WidgetComponent
{
    /**
     * @brief A class representing a text label in a graphical user interface.
     * 
     * This class manages the properties of a text label, including its font, 
     * text content, alignment, color, size, and position. It uses a rendering
     * pipeline for displaying the text.
     */
    class TextLabel
	{
    private:
        /**
         *  @brief Vertex Shader Module of the TextLabel Component.
         *
         *  This shader module is intended to handle the vertex shading logic
         *  specific to rendering a TextLabel Component in the graphics pipeline.
         */
        static spk::ShaderModule VertexShaderModule; 
        /**
         *  @brief Fragment Shader Module of the TextLabel Component.
         *
         *  This shader module is intended to handle the fragment shading logic
         *  specific to rendering a TextLabel Component in the graphics pipeline.
         */
        static spk::ShaderModule FragmentShaderModule; 

        /**
         * @brief Shared pointer to the rendering pipeline used by all TextLabel instances.
         */
        static inline std::shared_ptr<spk::Pipeline> _renderingPipeline = nullptr;
        static inline std::shared_ptr<spk::Pipeline::SamplerUniform> _renderingPipelineTextureIDUniform = nullptr;

        /**
         * @brief Shared pointer to the rendering object for this particular text label.
         */
        std::shared_ptr<spk::Pipeline::Object> _renderingObject;

        /**
         * @brief A wrapper for the font used in the text label.
         * 
         * This variable holds a shared pointer to the `Font` object, allowing the text label
         * to use different fonts. The `ValueWrapper` ensures that changes to the font can be
         * tracked and handled appropriately.
         */
        ValueWrapper<std::shared_ptr<Font>> _font;

        /**
         * @brief Shared pointer to the font atlas.
         * 
         * The font atlas contains the glyph data necessary for rendering the text.
         * This is used in conjunction with the font to render the text label.
         */
        std::shared_ptr<Font::Atlas> _fontAtlas;

        /**
         * @brief Wrapper for the text content of the label.
         * 
         * This variable holds the actual text to be displayed by the label. The `ValueWrapper`
         * allows for efficient tracking and updating of the text content.
         */
        ValueWrapper<std::wstring> _text;

        /**
         * @brief Wrapper for the vertical alignment of the text within the label.
         * 
         * This variable determines how the text is aligned vertically (top, middle, bottom)
         * within the label's area.
         */
        ValueWrapper<Font::VerticalAlignment> _verticalAlignment;

        /**
         * @brief Wrapper for the horizontal alignment of the text within the label.
         * 
         * This variable controls the horizontal alignment (left, center, right) of the text
         * within the label's area.
         */
        ValueWrapper<Font::HorizontalAlignment> _horizontalAlignment;

        /**
         * @brief Wrapper for the color of the text.
         * 
         * This variable specifies the color used to render the text of the label.
         */
        ValueWrapper<spk::Color> _textColor;

        /**
         * @brief Wrapper for the size of the text.
         * 
         * This variable determines the size of the text in the label, affecting how large
         * the text is rendered.
         */
        ValueWrapper<size_t> _textSize;

        /**
         * @brief Wrapper for the type of outline used around the text.
         * 
         * This variable specifies the style of the outline drawn around the text, if any.
         */
        ValueWrapper<Font::OutlineType> _outlineType;

        /**
         * @brief Wrapper for the color of the text outline.
         * 
         * This variable specifies the color of the outline that may be drawn around the text.
         */
        ValueWrapper<spk::Color> _outlineColor;

        /**
         * @brief Wrapper for the size of the text outline.
         * 
         * This variable determines the thickness of the outline drawn around the text.
         */
        ValueWrapper<size_t> _outlineSize;

        /**
         * @brief Wrapper for the anchor point of the text label.
         * 
         * This variable specifies the anchor point in terms of a 2D vector, which affects the
         * positioning of the label in the UI.
         */
        ValueWrapper<spk::Vector2Int> _anchor;

        /**
         * @brief Wrapper for the depth of the text label.
         * 
         * This variable determines the depth at which the label is rendered, useful in a 3D
         * rendering context or for layering purposes.
         */
        ValueWrapper<float> _depth;

        /**
         * @brief Structure to hold rendering data for the text label.
         * 
         * This structure contains necessary information for rendering the text label,
         * including its size, position, and the glyph data for each character in the text.
         */
        struct RenderData
        {
            /**
             * @brief The size of the text label in 2D space.
             */
            spk::Vector2Int size;

            /**
             * @brief The top-left corner position of the text label.
             */
            spk::Vector2Int topLeftCorner;

            /**
             * @brief The bottom-right corner position of the text label.
             */
            spk::Vector2Int downRightCorner;

            /**
             * @brief A vector of glyph data pointers, each representing a character in the text.
             */
            std::vector<const spk::Font::Atlas::GlyphData *> glyphDatas;
        };

        /**
         * @brief Computes and returns the rendering data for the text label.
         * 
         * This function calculates the size, position, and glyph data for the current text
         * in the label, based on its properties like font, size, and alignment.
         * 
         * @return RenderData The computed rendering data for the text label.
         */
        RenderData _computeRenderData();

        /**
         * @brief Computes the offset for positioning the string based on the provided render data.
         * 
         * Given render data, this function calculates the offset required to position the text
         * correctly within the label according to its alignment properties.
         * 
         * @param p_renderData The render data used for calculating the offset.
         * @return spk::Vector2Int The calculated offset for the string.
         */
        spk::Vector2Int _computeStringOffset(const RenderData& p_renderData);

        /**
         * @brief Updates the vertices of the text label for rendering.
         * 
         * This function recalculates the vertices necessary for rendering the text label,
         * based on its current properties and render data.
         */
        void _updateVertices();

        /**
         * @brief Updates the color of the text.
         * 
         * This function is called when the text color changes, ensuring that the new color
         * is applied to the text in the next render.
         */
        void _updateTextColor();

        /**
         * @brief Updates the color of the text outline.
         * 
         * Similar to text color updating, this function applies changes to the outline color
         * of the text, if an outline is used.
         */
        void _updateOutlineColor();

        /**
         * @brief Updates the depth property of the text label.
         * 
         * This function adjusts the depth at which the text label is rendered, affecting
         * its appearance in a 3D space or layering context.
         */
        void _updateDepth();

    public:
        /**
         * @brief Constructs a new TextLabel object.
         * 
         * This constructor initializes a new TextLabel with default properties. It sets up
         * the necessary rendering objects and prepares the label for rendering.
         */
        TextLabel();

        /**
         * @brief Renders the text label.
         * 
         * This function is responsible for drawing the text label on the screen. It updates
         * the rendering data based on the current properties of the label and then renders it
         * using the associated rendering pipeline.
         */
        void render();

        // Static inline default values for various text label properties

        /**
         * @brief Default font for all TextLabel instances.
         * 
         * This static value sets the default font used when creating new TextLabel instances.
         */
        static inline ValueWrapper<std::shared_ptr<Font>>::Default defaultFont = nullptr;

        /**
         * @brief Default text content for all TextLabel instances.
         * 
         * Sets the default string that is displayed by the TextLabel when it is first created.
         */
        static inline ValueWrapper<std::wstring>::Default defaultText = L"";

        /**
         * @brief Default vertical alignment for all TextLabel instances.
         * 
         * Specifies the default vertical alignment (top, middle, bottom) for new TextLabel instances.
         */
        static inline ValueWrapper<Font::VerticalAlignment>::Default defaultVerticalAlignment = Font::VerticalAlignment::Middle;

        /**
         * @brief Default horizontal alignment for all TextLabel instances.
         * 
         * Sets the default horizontal alignment (left, center, right) for new TextLabel instances.
         */
        static inline ValueWrapper<Font::HorizontalAlignment>::Default defaultHorizontalAlignment = Font::HorizontalAlignment::Middle;

        /**
         * @brief Default text color for all TextLabel instances.
         * 
         * Defines the default color used for the text in new TextLabel instances.
         */
        static inline ValueWrapper<spk::Color>::Default defaultTextColor = spk::Color(0, 0, 0, 255);

        /**
         * @brief Default text size for all TextLabel instances.
         * 
         * Determines the default size of the text in new TextLabel instances.
         */
        static inline ValueWrapper<size_t>::Default defaultTextSize = 0;

        /**
         * @brief Default outline type for all TextLabel instances.
         * 
         * Specifies the default style of the outline (if any) drawn around the text in new TextLabel instances.
         */
        static inline ValueWrapper<Font::OutlineType>::Default defaultOutlineType = Font::OutlineType::Standard;

        /**
         * @brief Default outline color for all TextLabel instances.
         * 
         * Sets the default color of the outline (if used) for new TextLabel instances.
         */
        static inline ValueWrapper<spk::Color>::Default defaultOutlineColor = spk::Color(0, 0, 0, 255);

        /**
         * @brief Default outline size for all TextLabel instances.
         * 
         * Defines the default thickness of the outline (if used) around the text in new TextLabel instances.
         */
        static inline ValueWrapper<size_t>::Default defaultOutlineSize = 0;

        /**
         * @brief Default anchor point for all TextLabel instances.
         * 
         * Specifies the default anchor point in terms of a 2D vector for new TextLabel instances.
         */
        static inline ValueWrapper<spk::Vector2Int>::Default defaultAnchor = spk::Vector2Int(0, 0);

        /**
         * @brief Default depth for all TextLabel instances.
         * 
         * Determines the default depth at which the label is rendered in a 3D context or for layering.
         */
        static inline ValueWrapper<float>::Default defaultDepth = 0.0f;

        /**
         * @brief Accessor for the font property.
         * 
         * @return Value<std::shared_ptr<Font>>& Reference to the font value wrapper.
         */
        Value<std::shared_ptr<Font>>& font() { return (_font.value()); }

        /**
         * @brief Accessor for the text property.
         * 
         * @return Value<std::wstring>& Reference to the text value wrapper.
         */
        Value<std::wstring>& text() { return (_text.value()); }

        /**
         * @brief Accessor for the vertical alignment property.
         * 
         * @return Value<Font::VerticalAlignment>& Reference to the vertical alignment value wrapper.
         */
        Value<Font::VerticalAlignment>& verticalAlignment() { return (_verticalAlignment.value()); }

        /**
         * @brief Accessor for the horizontal alignment property.
         * 
         * @return Value<Font::HorizontalAlignment>& Reference to the horizontal alignment value wrapper.
         */
        Value<Font::HorizontalAlignment>& horizontalAlignment() { return (_horizontalAlignment.value()); }

        /**
         * @brief Accessor for the text color property.
         * 
         * @return Value<spk::Color>& Reference to the text color value wrapper.
         */
        Value<spk::Color>& textColor() { return (_textColor.value()); }

        /**
         * @brief Accessor for the text size property.
         * 
         * @return Value<size_t>& Reference to the text size value wrapper.
         */
        Value<size_t>& textSize() { return (_textSize.value()); }

        /**
         * @brief Accessor for the outline type property.
         * 
         * @return Value<Font::OutlineType>& Reference to the outline type value wrapper.
         */
        Value<Font::OutlineType>& outlineType() { return (_outlineType.value()); }

        /**
         * @brief Accessor for the outline color property.
         * 
         * @return Value<spk::Color>& Reference to the outline color value wrapper.
         */
        Value<spk::Color>& outlineColor() { return (_outlineColor.value()); }

        /**
         * @brief Accessor for the outline size property.
         * 
         * @return Value<size_t>& Reference to the outline size value wrapper.
         */
        Value<size_t>& outlineSize() { return (_outlineSize.value()); }

        /**
         * @brief Accessor for the anchor property.
         * 
         * @return Value<spk::Vector2Int>& Reference to the anchor value wrapper.
         */
        Value<spk::Vector2Int>& anchor() { return (_anchor.value()); }

        /**
         * @brief Accessor for the depth property.
         * 
         * @return Value<float>& Reference to the depth value wrapper.
         */
        Value<float>& depth() { return (_depth.value()); }

        /**
         * @brief Const accessor for the font property.
         * 
         * @return const Value<std::shared_ptr<Font>>& Const reference to the font value wrapper.
         */
        const Value<std::shared_ptr<Font>>& font() const { return (_font.value()); }

        /**
         * @brief Const accessor for the text property.
         * 
         * @return const Value<std::wstring>& Const reference to the text value wrapper.
         */
        const Value<std::wstring>& text() const { return (_text.value()); }

        /**
         * @brief Const accessor for the vertical alignment property.
         * 
         * @return const Value<Font::VerticalAlignment>& Const reference to the vertical alignment value wrapper.
         */
        const Value<Font::VerticalAlignment>& verticalAlignment() const { return (_verticalAlignment.value()); }

        /**
         * @brief Const accessor for the horizontal alignment property.
         * 
         * @return const Value<Font::HorizontalAlignment>& Const reference to the horizontal alignment value wrapper.
         */
        const Value<Font::HorizontalAlignment>& horizontalAlignment() const { return (_horizontalAlignment.value()); }

        /**
         * @brief Const accessor for the text color property.
         * 
         * @return const Value<spk::Color>& Const reference to the text color value wrapper.
         */
        const Value<spk::Color>& textColor() const { return (_textColor.value()); }

        /**
         * @brief Const accessor for the text size property.
         * 
         * @return const Value<size_t>& Const reference to the text size value wrapper.
         */
        const Value<size_t>& textSize() const { return (_textSize.value()); }

        /**
         * @brief Const accessor for the outline type property.
         * 
         * @return const Value<Font::OutlineType>& Const reference to the outline type value wrapper.
         */
        const Value<Font::OutlineType>& outlineType() const { return (_outlineType.value()); }

        /**
         * @brief Const accessor for the outline color property.
         * 
         * @return const Value<spk::Color>& Const reference to the outline color value wrapper.
         */
        const Value<spk::Color>& outlineColor() const { return (_outlineColor.value()); }

        /**
         * @brief Const accessor for the outline size property.
         * 
         * @return const Value<size_t>& Const reference to the outline size value wrapper.
         */
        const Value<size_t>& outlineSize() const { return (_outlineSize.value()); }

        /**
         * @brief Const accessor for the anchor property.
         * 
         * @return const Value<spk::Vector2Int>& Const reference to the anchor value wrapper.
         */
        const Value<spk::Vector2Int>& anchor() const { return (_anchor.value()); }

        /**
         * @brief Const accessor for the depth property.
         * 
         * @return const Value<float>& Const reference to the depth value wrapper.
         */
        const Value<float>& depth() const { return (_depth.value()); }

    };
}