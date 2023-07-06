#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "math/spk_vector2.hpp"
#include "spk_system_define.hpp"

namespace spk
{
    /**
     * @class Window
     * @brief Singleton class for window.
     *
     * Represents the window of the application, providing functionality to modify its geometry, render content, and clear its content.
     */
	class Window : public spk::Singleton<Window>
	{
		friend class spk::Singleton<Window>;
		friend class APIModule;

	private:
        /**
         * @brief Window size.
         */
		spk::Vector2Int _size;

        /**
         * @brief Connection to the X server.
         */
		xcb_connection_t *_connection;

        /**
         * @brief Screen of the X server.
         */
		xcb_screen_t *_screen;

        /**
         * @brief Window of the X server.
         */
		xcb_window_t _window;

        /**
         * @brief Creates a window.
         */
		void _createWindow();

        /**
         * @brief Names the window.
         *
         * @param p_title The title of the window.
         */
		void _nameWindow(const std::wstring& p_title);

        /**
         * @brief Constructs the Window object.
         *
         * @param p_title The title of the window.
         * @param p_size The size of the window.
         */
		Window(const std::wstring& p_title, const spk::Vector2Int& p_size);

	public:
        /**
         * @brief Sets the window geometry.
         *
         * @param p_size The size to set for the window.
         */
		void setGeometry(const spk::Vector2Int& p_size);

        /**
         * @brief Resizes the window.
         *
         * @param p_size The size to set for the window.
         */
		void resize(const spk::Vector2Int& p_size);

        /**
         * @brief Renders the window.
         */
		void render();

        /**
         * @brief Clears the window.
         */
		void clear();
	};
}
