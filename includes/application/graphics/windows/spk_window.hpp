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

		HINSTANCE _hInstance;

		WNDCLASS _windowClass;
		
		RECT _windowSize;
		DWORD _windowStyle;
		DWORD _windowExStyle;

		HWND _windowFrame;

		Window(spk::Vector2Int p_size, void *p_ptr = nullptr);

	public:
		void setGeometry(spk::Vector2Int p_size);
		void resize(spk::Vector2Int p_size);
		void render();
		void clear();
	};
}