#pragma once

#include "graphics/spk_frame.hpp"
#include "graphics/spk_surface.hpp"

namespace spk
{
    /**
     * Singleton class representing a window.
     */
    class Window : public spk::Singleton<Window>
    {
        // Allow the Singleton to access Window's private constructor.
        friend class spk::Singleton<Window>;

    private:
        spk::Frame _frame;  /**< Frame object to manage the window frame. */
        spk::Surface _surface;  /**< Surface object to manage the rendering surface. */

        /**
         * Private constructor for Window.
         * Can only be accessed by Singleton<Window>.
         */
        Window(const std::wstring &p_title, const Vector2UInt &p_size, void *p_ptr = nullptr);

    public:
        // Deleting the copy constructor and copy assignment operator.
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        // Default move constructor and move assignment operator.
        Window(Window&&) = default;
        Window& operator=(Window&&) = default;

        /**
         * Clears the window's frame and surface.
         */
        void clear();

        /**
         * Initiates the rendering process.
         */
        void render();

        /**
         * Sets the size of the window.
         */
        void setSize(const spk::Vector2UInt& p_size);

        /**
         * Resizes the window and its components.
         */
        void resize(const spk::Vector2UInt& p_size);

        /**
         * Retrieves the current size of the window.
         */
        const spk::Vector2UInt& size() const;

        /**
         * Retrieves the Frame object managing the window frame.
         */
        const spk::Frame& frame() const;
    };
}
