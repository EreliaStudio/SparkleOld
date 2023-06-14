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

		wchar_t* _convertedTitle;
		HINSTANCE _hInstance;
		HWND _windowFrame;
		RECT _windowSize;
		WNDCLASS _windowClass;
		DWORD _windowStyle;
		DWORD _windowExStyle;

		void _convertTitle(const std::wstring& p_title);
		void _createGhostInstance();
		void _registerWindowClass();
		void _createWindowFrame(void* p_APIModule, const spk::Vector2Int& p_size);
		void _composeOpenGLContext();
		void _activateWindow();

		Window(const std::wstring& p_title, const spk::Vector2Int& p_size, void *p_ptr = nullptr);

	public:
		~Window();
		void setGeometry(const spk::Vector2Int& p_size);
		void resize(const spk::Vector2Int& p_size);
		void render();
		void clear();
	};
}