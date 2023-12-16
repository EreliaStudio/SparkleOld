#pragma once

#include "graphics/spk_frame.hpp"
#include "graphics/spk_surface.hpp"

namespace spk
{
    /**
     * Singleton class representing a window.
     */
    class Window
    {
    private:
        spk::Frame _frame;  /**< Frame object to manage the window frame. */
        spk::Surface _surface;  /**< Surface object to manage the rendering surface. */


    public:
        /**
         * Constructor for Window.
         * Can only be accessed by Singleton<Window>.
         */
        Window(const std::wstring &p_title, const Vector2UInt &p_size, void *p_ptr = nullptr);

        /**
         * @brief Deleted copy constructor.
         *
         * Copying instances of this class is not allowed to ensure that resources
         * managed by the Window are not duplicated or improperly managed.
         */
        Window(const Window&) = delete;

        /**
         * @brief Deleted copy assignment operator.
         *
         * Copy assignment is not allowed to ensure that resources managed by the
         * Window are not duplicated or improperly managed.
         */
        Window& operator=(const Window&) = delete;

        /**
         * @brief Default move constructor.
         *
         * Moving instances of this class is allowed. Resources are transferred from
         * the source instance to the new instance.
         */
        Window(Window&&) = default;

        /**
         * @brief Default move assignment operator.
         *
         * Move assignment is allowed. Resources are transferred from the source
         * instance to the this instance, replacing the current resources.
         */
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
         * @brief Places the window at a specified anchor point.
         *
         * Sets the position of the window on the screen to the given anchor point.
         *
         * @param p_anchor The position to place the window at, as a Vector2Int.
         */
        void place(const spk::Vector2Int& p_anchor);

        /**
         * @brief Retrieves the current anchor position of the window.
         *
         * @return The current anchor position of the window as a Vector2Int.
         */
        const spk::Vector2Int& anchor() const;

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
