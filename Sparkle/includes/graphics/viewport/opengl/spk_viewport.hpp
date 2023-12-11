#pragma once

#include "graphics/viewport/spk_abstract_viewport.hpp"

namespace spk
{
    /**
     *  @class Viewport
     *  @brief A concrete implementation of the spk::AbstractViewport class.
     *  @extends spk::AbstractViewport
     *
     *  This class provides a concrete implementation for handling viewports,
     *  inheriting the common functionalities from spk::AbstractViewport.
     */
	class Viewport : public spk::AbstractViewport
	{
	private:
        /**
         *  @brief Handles viewport activation and performs necessary operations.
         *  @param p_area The area to activate for this viewport.
         *
         *  This function is the concrete implementation of the pure virtual function
         *  _onActivation() defined in the spk::AbstractViewport class. It specifies
         *  what happens when this viewport is activated.
         */
		void _onActivation(const spk::Area& p_area) const override;

	public:
        /// Default constructor
		Viewport();
	};
}