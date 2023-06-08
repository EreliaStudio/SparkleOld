#include "application/linux/graphics/spk_window.hpp"

namespace spk
{
	Window::Window(size_t width, size_t height)
	{
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
			width, height,
			10,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			_screen->root_visual,
			mask, values);

		xcb_map_window(_connection, _window);
	}

	void Window::render()
	{
		xcb_flush(_connection);
	}

	void Window::clear()
	{
		xcb_clear_area(_connection, 0, _window, 0, 0, 800, 800);
	}
}