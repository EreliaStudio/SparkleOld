#pragma once

#include "design_pattern/spk_value.hpp"
#include "graphics/spk_color.hpp"
#include "graphics/spk_area.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk::WidgetComponent
{
    /**
 * @brief Class representing a graphical box element.
 * 
 * This class manages the graphical properties and rendering of a box, including
 * its color, size, depth, and border size. It uses a pipeline for rendering and
 * handles updates to its properties.
 */
class Box
{
private:
    /**
     * @brief Shared pointer to the rendering pipeline used by all Box instances.
     */
    static inline std::shared_ptr<spk::Pipeline> _renderingPipeline = nullptr;

    /**
     * @brief Index for background in the rendering objects array.
     */
    static const size_t BackgroundIndex = 0;

    /**
     * @brief Index for frontground in the rendering objects array.
     */
    static const size_t FrontgroundIndex = 1;

    /**
     * @brief Array of rendering objects for the background and frontground.
     */
    std::shared_ptr<spk::Pipeline::Object> _renderingObjects[2];

    /**
     * @brief Wrapper for the color of the frontground.
     */
    ValueWrapper<spk::Color> _frontgroundColor;

    /**
     * @brief Wrapper for the color of the background.
     */
    ValueWrapper<spk::Color> _backgroundColor;

    /**
     * @brief Wrapper for the border size of the box.
     */
    ValueWrapper<size_t> _borderSize;

    /**
     * @brief Wrapper for the depth of the box.
     */
    ValueWrapper<float> _depth;

    /**
     * @brief Wrapper for the area of the box.
     */
    ValueWrapper<spk::Area> _area;

    /**
     * @brief Updates the background color of the box.
     * 
     * This function is responsible for handling changes in the background color
     * of the box. It typically gets called when the background color value changes.
     * It ensures that the new color is applied to the box's background.
     */
    void _updateBackgroundColor();

    /**
     * @brief Updates the frontground color of the box.
     * 
     * This function handles the updates to the frontground color. It is invoked
     * whenever there is a change in the frontground color value. The function
     * ensures the box's frontground reflects the new color appropriately.
     */
    void _updateFrontgroundColor();

    /**
     * @brief Updates the depth of the box.
     * 
     * The depth update function adjusts the box's depth property. It is called
     * when there's a change in the depth value. This function ensures that the 
     * box's depth is set to the new value, affecting its rendering.
     */
    void _updateDepth();

    /**
     * @brief Updates the vertices for the frontground of the box.
     * 
     * This function recalculates the vertices for the frontground of the box.
     * It is typically called when there's a change in the box's properties that
     * affects its frontground shape or position.
     */
    void _updateFrontgroundVertices();

    /**
     * @brief Updates the vertices for the background of the box.
     * 
     * Similar to the frontground vertices update, this function recalculates
     * the vertices for the background of the box. It is invoked when changes
     * in the box's properties affect the background's geometry.
     */
    void _updateBackgroundVertices();

    /**
     * @brief General function to update the vertices of the box.
     * 
     * This function is a more general vertex update function. It calls the
     * specific frontground and background vertex update functions. It's used
     * when there is a need to update all vertices of the box, such as during
     * a complete redraw or significant property change.
     */
    void _updateVertices();


public:
    /**
     * @brief Constructs a new Box object with default properties.
     */
    Box();

    /**
     * @brief Renders the box using the specified pipeline.
     */
    void render();

    /**
     * @brief Default frontground color for all Box instances.
     * 
     * This static value sets the default frontground color used when creating
     * new Box instances, unless specified otherwise.
     */
    static inline ValueWrapper<spk::Color>::Default defaultFrontgroundColor = spk::Color(180, 180, 180, 255);

    /**
     * @brief Default background color for all Box instances.
     * 
     * Similar to the default frontground color, this static value sets the
     * default background color for new Box instances.
     */
    static inline ValueWrapper<spk::Color>::Default defaultBackgroundColor = spk::Color(120, 120, 120, 255);

    /**
     * @brief Default border size for all Box instances.
     * 
     * Sets the default border size for Box instances, defining the thickness
     * of the border around the box.
     */
    static inline ValueWrapper<size_t>::Default defaultBorderSize = 1;

    /**
     * @brief Default depth for all Box instances.
     * 
     * Specifies the default depth value for new Box instances, affecting
     * how the box is rendered in a 3D space.
     */
    static inline ValueWrapper<float>::Default defaultDepth = 1.0f;

    /**
     * @brief Default area for all Box instances.
     * 
     * Defines the default area (size and position) for new Box instances,
     * with a starting point of (0, 0) and a default size of 100x100 units.
     */
    static inline ValueWrapper<spk::Area>::Default defaultArea = { spk::Vector2Int(0, 0), spk::Vector2UInt(100, 100) };

    // Member functions to access and manipulate box properties

    /**
     * @brief Provides access to the current frontground color.
     * 
     * @return Value<spk::Color>& Reference to the frontground color value.
     */
    Value<spk::Color>& frontgroundColor() { return (_frontgroundColor.value()); }

    /**
     * @brief Provides access to the current background color.
     * 
     * @return Value<spk::Color>& Reference to the background color value.
     */
    Value<spk::Color>& backgroundColor() { return (_backgroundColor.value()); }

    /**
     * @brief Provides access to the current border size.
     * 
     * @return Value<size_t>& Reference to the border size value.
     */
    Value<size_t>& borderSize() { return (_borderSize.value()); }

    /**
     * @brief Provides access to the current depth of the box.
     * 
     * @return Value<float>& Reference to the depth value.
     */
    Value<float>& depth() { return (_depth.value()); }

    /**
     * @brief Provides access to the current area of the box.
     * 
     * @return Value<spk::Area>& Reference to the area value.
     */
    Value<spk::Area>& area() { return (_area.value()); }

    /**
     * @brief Provides const access to the current frontground color.
     * 
     * This function returns a constant reference to the frontground color
     * of the box. It's used in contexts where the box's state should not be
     * modified.
     * 
     * @return const Value<spk::Color>& Const reference to the frontground color value.
     */
    const Value<spk::Color>& frontgroundColor() const { return (_frontgroundColor.value()); }

    /**
     * @brief Provides const access to the current background color.
     * 
     * Similar to the frontground color access, this function returns a constant
     * reference to the background color. It's used where the box is constant and
     * its state should not be altered.
     * 
     * @return const Value<spk::Color>& Const reference to the background color value.
     */
    const Value<spk::Color>& backgroundColor() const { return (_backgroundColor.value()); }

    /**
     * @brief Provides const access to the current border size.
     * 
     * This function returns a constant reference to the border size of the box.
     * It's particularly useful in read-only contexts where the box's border size
     * needs to be accessed without modifying it.
     * 
     * @return const Value<size_t>& Const reference to the border size value.
     */
    const Value<size_t>& borderSize() const { return (_borderSize.value()); }

    /**
     * @brief Provides const access to the current depth of the box.
     * 
     * Returns a constant reference to the depth property of the box. This function
     * is used when the depth needs to be accessed in a read-only manner.
     * 
     * @return const Value<float>& Const reference to the depth value.
     */
    const Value<float>& depth() const { return (_depth.value()); }

    /**
     * @brief Provides const access to the current area of the box.
     * 
     * This function offers a constant reference to the box's area, allowing it
     * to be accessed in contexts where the box is treated as constant and its
     * properties should not be modified.
     * 
     * @return const Value<spk::Area>& Const reference to the area value.
     */
    const Value<spk::Area>& area() const { return (_area.value()); }

};

}