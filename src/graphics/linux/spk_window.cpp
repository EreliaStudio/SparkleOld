#include "graphics/spk_window.hpp"
#include "iostream/spk_iostream.hpp"
#include "application/modules/spk_API_module.hpp"

#define DEBUG_LINE() spk::cout << __FUNCTION__ << "::" << __LINE__ << std::endl

namespace spk
{
	Window::Window(const std::wstring& p_title, spk::Vector2Int p_size)
	{
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

		std::string title_str(p_title.begin(), p_title.end());

		xcb_change_property(_connection,
			XCB_PROP_MODE_REPLACE,
			_window,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING,
			8,
			title_str.size(),
			title_str.c_str());

		xcb_map_window(_connection, _window);
	}

	void Window::setGeometry(spk::Vector2Int p_size)
	{
		_size = p_size;

		uint32_t values[2] = { static_cast<uint32_t>(p_size.x), static_cast<uint32_t>(p_size.y) };
    	xcb_configure_window(_connection, _window, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
	}

	void Window::resize(spk::Vector2Int p_size)
	{
		_size = p_size;
	}

	void Window::render()
	{
		xcb_flush(_connection);
	}

	void Window::clear()
	{
		xcb_clear_area(_connection, 0, _window, 0, 0, _size.x, _size.y);
	}
}