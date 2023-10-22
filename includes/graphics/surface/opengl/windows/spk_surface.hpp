#pragma once

#include "graphics/surface/spk_abstract_surface.hpp"
#include "spk_system_define.hpp"

namespace spk
{
    /**
     * @class Surface
     * @brief Derived class for creating and managing a Windows-based OpenGL rendering surface.
     * @extends spk::AbstractSurface
     */
	class Surface : public spk::AbstractSurface
	{
	private:
        /// Handle to the OpenGL rendering context.
		HGLRC _hrc;

        /// Handle to the device context.
		HDC _hdc;

        /// The pixel format descriptor for the surface.
		PIXELFORMATDESCRIPTOR _pfd;

	public:
        /**
         * @brief Constructs a Surface object.
         * 
         * @param p_frame Pointer to the frame that owns this surface.
         */
		Surface(spk::AbstractFrame* p_frame);

        /**
         * @brief Resizes the surface to fit its container.
         */
        void resize();

        /**
         * @brief Clears the surface, reverting it to its initial state.
         */
		void clear();

        /**
         * @brief Renders the content onto the surface.
         */
		void render();
	};
}
