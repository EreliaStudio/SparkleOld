#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "graphics/surface/spk_abstract_surface.hpp"
#include "math/spk_vector2.hpp"
#include "spk_system_define.hpp"

namespace spk
{
	/**
	 * @brief Singleton class for window.
	 *
	 * Represents the window of the application, providing functionality to modify its geometry, render content, and clear its content.
	 */
	class Window : public Singleton<Window>
	{
		friend class Singleton<Window>;
		friend class APIModule;

	private:
		
		Vector2Int _size;///< Window size.
		wchar_t* _convertedTitle;///< Converted title for window.
		HINSTANCE _hInstance;///< Instance handle.
		HWND _windowFrame;///< Window handle.
		RECT _windowSize;///< Rectangle structure defining the window size.
		WNDCLASS _windowClass;///< Window class structure.
		DWORD _windowStyle;///< Window style.
		DWORD _windowExStyle;///< Window extended style.

		spk::AbstractSurface* _surface;

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
		void _createWindowFrame(void* p_APIModule, const Vector2Int& p_size);

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
		Window(const std::wstring& p_title, const Vector2Int& p_size, const GraphicalAPI& p_graphicalAPI, void *p_ptr = nullptr);

	public:
		/**
		 * @brief Destructs the Window object.
		 */
		~Window();
		
		void createSurface();

		/**
		 * @brief Resizes the window.
		 *
		 * @param p_size The size to set for the window.
		 */
		void resize(const Vector2Int& p_size);

		/**
		 * @brief Return the size of the window.
		*/
		const Vector2Int& size() const;

		AbstractSurface* surface();
		const AbstractSurface* surface() const;

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
