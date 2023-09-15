#include "graphics/spk_window.hpp"
#include "iostream/spk_iostream.hpp"
#include "application/modules/spk_API_module.hpp"

namespace spk
{
	void Window::_convertTitle(const std::wstring& p_title)
	{
		_convertedTitle = new wchar_t[p_title.size() + 1];
		for (size_t i = 0; i < p_title.size(); i++)
		{
			_convertedTitle[i] = p_title[i];
		}
		_convertedTitle[p_title.size()] = '\0';
	}
	void Window::_createGhostInstance()
	{
		_hInstance = GetModuleHandle(NULL);
	}
	void Window::_registerWindowClass()
	{
		_windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		_windowClass.lpfnWndProc = (WNDPROC)spk::APIModule::WindowProc;
		_windowClass.cbClsExtra = 0;
		_windowClass.cbWndExtra = 0;
		_windowClass.hInstance = _hInstance;
		_windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		_windowClass.hbrBackground = NULL;
		_windowClass.lpszMenuName = NULL;
		_windowClass.lpszClassName = _convertedTitle;

		RegisterClass(&_windowClass);
	}
  
	void Window::_createWindowFrame(void* p_APIModule, const spk::Vector2Int& p_size)
	{
		_windowSize.left = (long)0;
		_windowSize.right = (long)p_size.x;
		_windowSize.top = (long)0;
		_windowSize.bottom = (long)p_size.y;

		_windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		_windowStyle = WS_OVERLAPPEDWINDOW;

		AdjustWindowRectEx(&_windowSize, _windowStyle, FALSE, _windowExStyle);

		_windowFrame = CreateWindowEx(_windowExStyle, (LPCTSTR)(_convertedTitle), (LPCTSTR)(_convertedTitle), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, _windowSize.right - _windowSize.left, _windowSize.bottom - _windowSize.top, NULL, NULL, _hInstance, p_APIModule);
	}

	void Window::_activateWindow()
	{
		ShowWindow(_windowFrame, SW_SHOW);
		UpdateWindow(_windowFrame);
	}

	Window::Window(const std::wstring& p_title, const spk::Vector2Int& p_size, const GraphicalAPI& p_graphicalAPI, void* p_APIModule)
	{
		_convertTitle(p_title);

		_createGhostInstance();

		_registerWindowClass();

		_createWindowFrame(p_APIModule, p_size);

		_activateWindow();
	}

	Window::~Window()
	{
		delete _convertedTitle;
	}

	void Window::resize(const spk::Vector2Int& p_size)
	{
		_size = p_size;
	}

	const spk::Vector2Int& Window::size() const
	{
		return (_size);
	}

	void Window::render()
	{

	}

	void Window::clear()
	{
		InvalidateRect(_windowFrame, NULL, TRUE);
	}
}