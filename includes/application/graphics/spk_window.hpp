#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "math/spk_vector2.hpp"
#include "spk_system_define.hpp"

namespace spk
{
	class Window : public spk::Singleton<Window>
	{
		friend class spk::Singleton<Window>;
		friend class APIModule;

	private:
		spk::Vector2Int _size;

		#ifdef _WIN32

		#elif __linux__
			xcb_connection_t *_connection;
			xcb_screen_t *_screen;
			xcb_window_t _window;
		#endif

		Window(spk::Vector2Int p_size);

	public:
		void setGeometry(spk::Vector2Int p_size);
		void resize(spk::Vector2Int p_size);
		void render();
		void clear();
	};
}