#pragma once

#include "graphics/frame/spk_abstract_frame.hpp"

namespace spk
{
    /**
     * @brief An abstract class representing a surface for rendering.
     * 
     * This class serves as the base class for all surface types,
     * providing the essential interfaces for resizing, clearing, and rendering.
     */
	class AbstractSurface
	{
	protected:
        /// A pointer to the associated frame.
		const spk::AbstractFrame* _frame;

	public:
        /**
         * @brief Constructs an AbstractSurface associated with the given frame.
         * 
         * @param p_frame A pointer to the frame to be associated with this surface.
         */
		AbstractSurface(const spk::AbstractFrame* p_frame);

        // Deleted copy constructor and copy assignment operator
		AbstractSurface(const AbstractSurface&) = delete;
		AbstractSurface& operator=(const AbstractSurface&) = delete;

        // Default move constructor and move assignment operator
		AbstractSurface(AbstractSurface&&) = default;
		AbstractSurface& operator=(AbstractSurface&&) = default;

        /**
         * @brief Virtual function for resizing the surface.
         */
		virtual void resize() = 0;

        /**
         * @brief Virtual function for clearing the surface.
         */
		virtual void clear() = 0;

        /**
         * @brief Virtual function for rendering to the surface.
         */
		virtual void render() = 0;
	};
}
