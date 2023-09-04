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
		public:
		
		friend class Singleton<Window>;
		friend class APIModule;

	private:
		Vector2Int _size; ///< Window size.
		xcb_connection_t* _connection; ///< Connection to the X server.
		xcb_screen_t* _screen; ///< Screen of the X server.
		xcb_window_t _window; ///< Window of the X server.
		xcb_intern_atom_reply_t* _atom_wm_delete_window; ///< Atom for the window manager to close the window.

		spk::AbstractSurface* _surface;

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
		Window(const std::wstring& p_title, const Vector2Int& p_size, const GraphicalAPI& p_graphicalAPI, void *p_ptr = nullptr);

	public:

		/**
		 * @brief Destroys the Window object.
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
