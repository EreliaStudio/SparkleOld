#pragma once

#include "graphics/surface/spk_abstract_surface.hpp"
#include "graphics/spk_frame.hpp"
#include <GL/gl.h>
#include <GL/glx.h>

namespace spk
{
    /**
     * @class Surface
     * @brief Derived class for creating and managing a GLX-based surface.
     * @extends spk::AbstractSurface
     */
	class Surface : public spk::AbstractSurface
	{
	private:
        /// The GLX context associated with this surface.
		GLXContext _glxContext;

        /// The X11 display connected to this surface.
		Display* _display;

        /// The XCB connection for the X server.
		xcb_connection_t* _connection;

        /// The XCB window connected to this surface.
		xcb_window_t _window;

	public:
        /**
         * @brief Constructs a Surface object.
         * 
         * @param p_frame Pointer to the frame that owns this surface.
         */
		Surface(spk::Frame* p_frame);

        /**
         * @brief Resizes the surface.
         */
        void resize();

        /**
         * @brief Clears the surface, setting it to its initial state.
         */
		void clear();

        /**
         * @brief Renders the content on the surface.
         */
		void render();
	};
}
