#pragma once

#include "design_pattern/spk_singleton.hpp"
#include <xcb/xcb.h>

namespace spk
{
	class Window : public spk::Singleton<Window>
	{
		friend class spk::Singleton<Window>;
		friend class APIModule;

	private:
		xcb_connection_t *_connection;
		xcb_screen_t *_screen;
		xcb_window_t _window;

		Window(size_t width, size_t height);

	public:
		void render();
		void clear();
	};
}