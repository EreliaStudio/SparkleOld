#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "math/spk_vector2.hpp"
#include "spk_system_define.hpp"
#include "graphics/frame/spk_abstract_frame.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
    /**
     * @class Frame
     * @brief Concrete implementation of a frame using XCB for Linux platforms.
     * 
     * This class extends AbstractFrame and provides concrete implementations
     * specific to XCB.
     */
    class Frame : public spk::AbstractFrame
    {
        friend class APIModule; ///< Allow APIModule to access private members.

    private:
        /// Whether the window was resized.
        bool _wasResized = { false };

        /// Pointer to the X display structure.
        Display* _display;

        /// Default screen number.
        int _defaultScreen;

        /// Connection to the X server.
        xcb_connection_t* _connection;

        /// Screen data.
        xcb_screen_t* _screen;

        /// Window.
        xcb_window_t _window;

        /// Atom for delete window message.
        xcb_intern_atom_reply_t* _atom_wm_delete_window;

        /// Create the frame using XCB.
        void _createFrame();

        /**
         * @brief Set the name for the frame.
         * 
         * @param p_title The title to set.
         */
        void _nameFrame(const std::wstring& p_title);

        /**
         * @brief Enable delete window property.
         * 
         * @param p_connection Connection to X server.
         * @param p_window The window for which to set the property.
         * 
         * @return Pointer to the atom reply.
         */
        static xcb_intern_atom_reply_t* _enableDestroyXCBProperty(xcb_connection_t* p_connection, xcb_window_t p_window);

        /// Method for handling frame resizing logic.
        void _onResize();

        /// Method for handling setting the size of the frame.
        void _onSetSize();

    public:
        /**
         * @brief Frame constructor.
         * 
         * @param p_title Title of the frame.
         * @param p_size Initial size of the frame.
         * @param p_ptr Optional pointer to additional data (default is nullptr).
         */
        Frame(const std::wstring& p_title, const spk::Vector2UInt& p_size, void* p_ptr = nullptr);

        /// Destructor.
        ~Frame();

        /// Check if the window was resized.
        bool wasResized() const;

        /// Indicate that the resize has been handled.
        void resizeHandled();

        /// Get the current size of the frame.
        const spk::Vector2UInt& size() const;

        /// Render the frame.
        void render();

        /// Clear the frame.
        void clear();

        /// Get the display.
        Display* display() const { return (_display); }

        /// Get the default screen number.
        int defaultScreen() const { return (_defaultScreen); }

        /// Get the connection to the X server.
        xcb_connection_t* connection() const { return (_connection); }

        /// Get the window.
        xcb_window_t window() const { return (_window); }

        /// Get the atom for WM_DELETE_WINDOW.
        xcb_atom_t atomWmDeleteWindow() const { return (_atom_wm_delete_window->atom); }
    };
}
