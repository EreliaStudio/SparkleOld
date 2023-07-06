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
         * @brief Converted title for window.
         */
		wchar_t* _convertedTitle;

        /**
         * @brief Instance handle.
         */
		HINSTANCE _hInstance;

        /**
         * @brief Window handle.
         */
		HWND _windowFrame;

        /**
         * @brief Rectangle structure defining the window size.
         */
		RECT _windowSize;

        /**
         * @brief Window class structure.
         */
		WNDCLASS _windowClass;

        /**
         * @brief Window style.
         */
		DWORD _windowStyle;

        /**
         * @brief Window extended style.
         */
		DWORD _windowExStyle;

        /**
         * @brief Converts the title to a wchar_t.
         *
         * @param p_title The title of the window.
         */
		void _convertTitle(const std::wstring& p_title);

        /**
         * @brief Creates a ghost instance.
         */
		void _createGhostInstance();

        /**
         * @brief Registers the window class.
         */
		void _registerWindowClass();

        /**
         * @brief Creates the window frame.
         *
         * @param p_APIModule The API module.
         * @param p_size The size of the window.
         */
		void _createWindowFrame(void* p_APIModule, const spk::Vector2Int& p_size);

        /**
         * @brief Composes OpenGL context.
         */
		void _composeOpenGLContext();

        /**
         * @brief Activates the window.
         */
		void _activateWindow();

        /**
         * @brief Constructs the Window object.
         *
         * @param p_title The title of the window.
         * @param p_size The size of the window.
         * @param p_ptr An optional pointer (default is nullptr).
         */
		Window(const std::wstring& p_title, const spk::Vector2Int& p_size, void *p_ptr = nullptr);

	public:
        /**
         * @brief Destructs the Window object.
         */
		~Window();

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
