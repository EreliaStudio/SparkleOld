#include "application/graphics/spk_window.hpp"
#include "iostream/spk_iostream.hpp"
#include "application/modules/spk_API_module.hpp"

#define DEBUG_LINE() spk::cout << __FUNCTION__ << "::" << __LINE__ << std::endl

namespace spk
{
	Window::Window(spk::Vector2Int p_size, void* p_windowAPI)
	{
#ifdef _WIN32
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
			0, 0, _windowSize.right - _windowSize.left, _windowSize.bottom - _windowSize.top, NULL, NULL, _hInstance, NULL);

		DEBUG_LINE();
		ShowWindow(_windowFrame, SW_SHOW);
		UpdateWindow(_windowFrame);
		DEBUG_LINE();

#elif __linux__
		_size = p_size;
		_connection = xcb_connect(NULL, NULL);
		if (xcb_connection_has_error(_connection))
			throw std::runtime_error("Cannot open connection to the X server");

		_screen = xcb_setup_roots_iterator(xcb_get_setup(_connection)).data;

		_window = xcb_generate_id(_connection);

		uint32_t mask = XCB_CW_EVENT_MASK;
		uint32_t values[2] = {
			XCB_EVENT_MASK_KEY_PRESS |
			XCB_EVENT_MASK_KEY_RELEASE |
			XCB_EVENT_MASK_BUTTON_PRESS |
			XCB_EVENT_MASK_BUTTON_RELEASE |
			XCB_EVENT_MASK_ENTER_WINDOW |
			XCB_EVENT_MASK_LEAVE_WINDOW |
			XCB_EVENT_MASK_POINTER_MOTION |
			XCB_EVENT_MASK_BUTTON_1_MOTION |
			XCB_EVENT_MASK_BUTTON_2_MOTION |
			XCB_EVENT_MASK_BUTTON_3_MOTION |
			XCB_EVENT_MASK_BUTTON_4_MOTION |
			XCB_EVENT_MASK_BUTTON_5_MOTION |
			XCB_EVENT_MASK_RESIZE_REDIRECT,
			
			0};

		xcb_create_window(
			_connection,
			XCB_COPY_FROM_PARENT,
			_window,
			_screen->root,
			0, 0,
			p_size.x, p_size.y,
			10,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			_screen->root_visual,
			mask, values);

		xcb_map_window(_connection, _window);
#endif
	}

	void Window::setGeometry(spk::Vector2Int p_size)
	{
		_size = p_size;

#ifdef _WIN32

#elif __linux__
		uint32_t values[2] = { static_cast<uint32_t>(p_size.x), static_cast<uint32_t>(p_size.y) };
    	xcb_configure_window(_connection, _window, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
#endif

	}

	void Window::resize(spk::Vector2Int p_size)
	{
		_size = p_size;
	}

	void Window::render()
	{
#ifdef _WIN32

#elif __linux__
		xcb_flush(_connection);
#endif
	}

	void Window::clear()
	{
#ifdef _WIN32

#elif __linux__
		xcb_clear_area(_connection, 0, _window, 0, 0, _size.x, _size.y);
#endif
	}
}