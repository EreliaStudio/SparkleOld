#include "graphics/spk_window.hpp"
#include "iostream/spk_iostream.hpp"
#include "application/modules/spk_API_module.hpp"

#define DEBUG_LINE() spk::cout << __FUNCTION__ << "::" << __LINE__ << std::endl

namespace spk
{
	Window::Window(spk::Vector2Int p_size, void* p_windowAPI)
	{
		DEBUG_LINE();
		const char* tmp = "Nom de ma fenetre";

		DEBUG_LINE();
		_hInstance = GetModuleHandle(NULL);

		DEBUG_LINE();
		_windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		_windowClass.lpfnWndProc = (WNDPROC)spk::APIModule::WindowProc;
		_windowClass.cbClsExtra = 0;
		_windowClass.cbWndExtra = 0;
		_windowClass.hInstance = _hInstance;
		_windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		_windowClass.hbrBackground = NULL;
		_windowClass.lpszMenuName = NULL;

		DEBUG_LINE();
		_windowClass.lpszClassName = tmp;

		DEBUG_LINE();
		RegisterClass(&_windowClass);

		DEBUG_LINE();
		_windowSize.left = (long)0;
		_windowSize.right = (long)p_size.x;
		_windowSize.top = (long)0;
		_windowSize.bottom = (long)p_size.y;

		DEBUG_LINE();
		_windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		_windowStyle = WS_OVERLAPPEDWINDOW;

		DEBUG_LINE();
		AdjustWindowRectEx(&_windowSize, _windowStyle, FALSE, _windowExStyle);
		
		DEBUG_LINE();
		_windowFrame = CreateWindowEx(_windowExStyle, tmp, tmp, WS_OVERLAPPEDWINDOW,
			0, 0, _windowSize.right - _windowSize.left, _windowSize.bottom - _windowSize.top, NULL, NULL, _hInstance, p_windowAPI);

		DEBUG_LINE();
		ShowWindow(_windowFrame, SW_SHOW);
		UpdateWindow(_windowFrame);
		DEBUG_LINE();

		_size = p_size;
	}

	void Window::setGeometry(spk::Vector2Int p_size)
	{
		_size = p_size;
	}

	void Window::resize(spk::Vector2Int p_size)
	{
		_size = p_size;
	}

	void Window::render()
	{
	}

	void Window::clear()
	{
	}
}