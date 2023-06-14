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

		xcb_connection_t *_connection;
		xcb_screen_t *_screen;
		xcb_window_t _window;

		void _createWindow();
		void _nameWindow(const std::wstring& p_title);
		
		Window(const std::wstring& p_title, const spk::Vector2Int& p_size);

	public:
		void setGeometry(const spk::Vector2Int& p_size);
		void resize(const spk::Vector2Int& p_size);
		void render();
		void clear();
	};
}