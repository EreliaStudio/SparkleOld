#pragma once

#include "design_pattern/spk_value.hpp"
#include "graphics/spk_color.hpp"
#include "graphics/spk_area.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk::WidgetComponent
{
    /**
     *  @class Box
     *  @brief Class for managing box-shaped widget components.
     *  
     *  This class handles the rendering, geometry, and colors associated with a box-shaped widget.
     */
    class Box
    {
    private:
        static inline std::shared_ptr<spk::Pipeline> _renderingPipeline = nullptr; ///< Shared pipeline for rendering all Box objects

        static inline const size_t BackgroundIndex = 0; ///< Index for background rendering objects
        static inline const size_t BorderIndex = 1; ///< Index for border rendering objects

        std::shared_ptr<spk::Pipeline::Object> _renderingObjects[2]; ///< Rendering objects for the background and the border

        static inline spk::Value<Area>::Default _defaultArea = spk::Area(0, 0); ///< Default attribute area
        static inline spk::Value<float>::Default _defaultDepth = 0.0f; ///< Default attribute depth
        static inline spk::Value<spk::Color>::Default _defaultBackgroundColor = spk::Color(120, 120, 120); ///< Default attribute background color
        static inline spk::Value<spk::Color>::Default _defaultBorderColor = spk::Color(150, 150, 150); ///< Default attribute border color
        static inline spk::Value<size_t>::Default _defaultBorderSize = 5; ///< Default attribute border size

        /**
         *  @struct Attribute
         *  @brief Templated structure for managing attributes and their updates.
         *  @tparam TType The type of the attribute.
         */
        template<typename TType>
        struct Attribute
        {
            using AttributeType = spk::Value<TType>;

            ///< Mutex for thread-safe attribute updates
            std::recursive_mutex mutex;

            ///< Flag indicating whether the attribute needs an update
            bool needUpdate;

            ///< The attribute value
            AttributeType value;
            ///< The attribute contract
            AttributeType::Contract contract;

            /**
             *  @brief Constructor initializing the attribute with a default value.
             *  @param p_defaultValue The default value for the attribute.
             */
            Attribute(const typename AttributeType::Default& p_defaultValue) :
				needUpdate(true),
				value(p_defaultValue),
				contract(value.subscribe([&](){
						std::lock_guard<std::recursive_mutex> lockGuard(mutex);
						needUpdate = true;
					}))
			{

			}

            /**
             *  @brief Assignment operator for setting attribute values.
             *  @param p_value The value to set.
             *  @return Reference to this Attribute.
             */
			Attribute& operator = (const TType& p_value)
			{
				value = p_value;
				return (*this);
			}
        };

        /**
         *  @brief Attribute object for managing the area dimensions of the box.
         *  
         *  This attribute holds the geometric area dimensions of the box, including position and size.
         *  Whenever the area is updated, the `_updateArea()` method should be called.
         */
        Attribute<Area> _area;

        /**
         *  @brief Attribute object for managing the rendering depth of the box.
         *  
         *  This attribute determines the depth at which the box will be rendered.
         *  If the depth is updated, `_updateDepth()` method should be called.
         */
        Attribute<float> _depth;

        /**
         *  @brief Attribute object for managing the background color of the box.
         *  
         *  This attribute holds the RGB/RGBA color information for the background of the box.
         *  If the background color changes, `_updateBackgroundColor()` should be called.
         */
        Attribute<Color> _backgroundColor;

        /**
         *  @brief Attribute object for managing the border color of the box.
         *  
         *  This attribute holds the RGB/RGBA color information for the border of the box.
         *  If the border color changes, `_updateBorderColor()` should be called.
         */
        Attribute<Color> _borderColor;

        /**
         *  @brief Attribute object for managing the border size of the box.
         *  
         *  This attribute holds the size information for the border of the box.
         *  If the border size changes, `_updateBorderSize()` should be called.
         */
        Attribute<size_t> _borderSize;

        /**
         *  @brief Composes the geometry for the background rendering object.
         *  
         *  This method computes and sets the vertices, indices, and other geometric data
         *  for the background of the box based on its current area dimensions.
         */
        void _composeBackgroundObjectGeometry();

        /**
         *  @brief Composes the geometry for the border rendering object.
         *  
         *  This method computes and sets the vertices, indices, and other geometric data
         *  for the border of the box based on its current border size and area dimensions.
         */
        void _composeBorderObjectGeometry();

        /**
         *  @brief Updates the area dimensions.
         *  
         *  This method updates the area-related aspects of the box, such as repositioning
         *  and resizing. It's called whenever `_area` attribute is updated.
         */
        void _updateArea();

        /**
         *  @brief Updates the rendering depth.
         *  
         *  This method updates the depth at which the box is rendered.
         *  It's called whenever `_depth` attribute is updated.
         */
        void _updateDepth();

        /**
         *  @brief Updates the background color.
         *  
         *  This method updates the background color of the box.
         *  It's called whenever `_backgroundColor` attribute is updated.
         */
        void _updateBackgroundColor();

        /**
         *  @brief Updates the border color.
         *  
         *  This method updates the border color of the box.
         *  It's called whenever `_borderColor` attribute is updated.
         */
        void _updateBorderColor();

        /**
         *  @brief Updates the border size.
         *  
         *  This method updates the size of the border around the box.
         *  It's called whenever `_borderSize` attribute is updated.
         */
        void _updateBorderSize();

    public:
        /// Default constructor
        Box();

        /**
         *  @brief Sets the background color.
         *  @param p_backgroundColor The new background color.
         */
        void setBackgroundColor(const spk::Color& p_backgroundColor);

        /**
         *  @brief Sets the border color.
         *  @param p_borderColor The new border color.
         */
        void setBorderColor(const spk::Color& p_borderColor);

        /**
         *  @brief Sets both the background and border colors.
         *  @param p_backgroundColor The new background color.
         *  @param p_borderColor The new border color.
         */
        void setColors(const spk::Color& p_backgroundColor, const spk::Color& p_borderColor);

        /**
         *  @brief Sets the geometry based on the area.
         *  @param p_area The new area.
         */
        void setGeometry(const spk::Area& p_area);

        /**
         *  @brief Sets the geometry based on the border size.
         *  @param p_borderSize The new border size.
         */
        void setGeometry(const size_t& p_borderSize);

        /**
         *  @brief Sets both the area and border size for the geometry.
         *  @param p_area The new area.
         *  @param p_borderSize The new border size.
         */
        void setGeometry(const spk::Area& p_area, const size_t& p_borderSize);

        /**
         *  @brief Sets the depth for rendering.
         *  @param p_depth The new depth.
         */
        void setDepth(const float& p_depth);

        /**
         *  @brief Method to render the box component
         */
        void render();
    };
}