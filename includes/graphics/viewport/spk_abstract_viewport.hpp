#pragma once

#include "graphics/spk_area.hpp"
#include "design_pattern/spk_inherence_object.hpp"

namespace spk
{
    /**
     *  @class AbstractViewport
     *  @brief An abstract base class for handling viewports within the Sparkle library.
     */
	class AbstractViewport : public spk::InherenceObject<AbstractViewport>
	{
	private:
        /// Static variable to hold the active viewport area
		static inline Area _activeViewport;

        /// The area covered by this viewport
		Area _area;

        /**
         *  @brief Virtual function to handle viewport activation.
         *  @param p_area The area to activate.
         */
		virtual void _onActivation(const Area& p_area) const = 0;

        /**
         *  @brief Computes the active abstract viewport.
         *  @return The computed active viewport area.
         */
		Area _computeActiveAbstractViewport() const;

	public:
        /// Default constructor
		AbstractViewport();

        /**
         *  @brief Get the area covered by this viewport.
         *  @return A reference to the area.
         */
		constexpr Area& area()
		{
			return (_area);
		}

        /**
         *  @brief Get the area covered by this viewport (const overload).
         *  @return A constant reference to the area.
         */
		constexpr const Area& area() const
		{
			return (_area);
		}

        /**
         *  @brief Set the geometry for the viewport.
         *  @param p_area The new area to set.
         */
		void setGeometry(const Area& p_area);

        /**
         *  @brief Set the anchor point for the viewport.
         *  @param p_anchor The new anchor point.
         */
		void setAnchor(const spk::Vector2Int& p_anchor);

        /**
         *  @brief Set the size of the viewport.
         *  @param p_size The new size.
         */
		void setSize(const spk::Vector2UInt& p_size);

        /// Activate this viewport
		void activate() const;

        /**
         *  @brief Check if a given position is inside the viewport.
         *  @param p_position The position to check.
         *  @return True if the position is inside the viewport, false otherwise.
         */
		bool isInside(const spk::Vector2Int& p_position);

        /**
         *  @brief Get the currently active viewport.
         *  @return A constant reference to the active viewport.
         */
		static const spk::Area& activeViewport();

        /**
         *  @brief Reset the viewport to a new base size.
         *  @param p_baseViewportSize The new base size.
         */
		static void resetViewport(const spk::Vector2UInt& p_baseViewportSize);

        /**
         *  @brief Convert screen coordinates to GPU coordinates.
         *  @param p_screenPosition The screen position to convert.
         *  @return The corresponding GPU coordinates.
         */
		static spk::Vector2 convertScreenToGPUCoordinates(const spk::Vector2Int& p_screenPosition);
	};
}