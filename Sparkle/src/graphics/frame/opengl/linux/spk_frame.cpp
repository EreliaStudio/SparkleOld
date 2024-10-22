#include "graphics/frame/spk_frame.hpp"

namespace spk
{
	void Frame::_createFrame()
	{
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
		XCB_EVENT_MASK_STRUCTURE_NOTIFY,
		0 };

		xcb_create_window(
			_connection,
			XCB_COPY_FROM_PARENT,
			_window,
			_screen->root,
			0, 0,
			_size.x, _size.y,
			10,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			_screen->root_visual,
			mask, values);
	}

	void Frame::_nameFrame(const std::wstring& p_title)
	{
		std::string title_str(p_title.begin(), p_title.end());

		xcb_change_property(_connection,
			XCB_PROP_MODE_REPLACE,
			_window,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING,
			8,
			title_str.size(),
			title_str.c_str());
	}

	xcb_intern_atom_reply_t* Frame::_enableDestroyXCBProperty(xcb_connection_t* p_connection, xcb_window_t p_window)
	{
		xcb_intern_atom_cookie_t cookie = xcb_intern_atom(p_connection, 1, 12, "WM_PROTOCOLS");
		xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(p_connection, 0, 16, "WM_DELETE_WINDOW");

		xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(p_connection, cookie, 0);
		xcb_intern_atom_reply_t* reply2 = xcb_intern_atom_reply(p_connection, cookie2, 0);

		xcb_change_property(p_connection, XCB_PROP_MODE_REPLACE, p_window, (*reply).atom, 4, 32, 1, &(*reply2).atom);
		free(reply);

		return reply2;
	}

	void Frame::_onResize()
	{
		uint32_t values[2] = { static_cast<uint32_t>(_size.x), static_cast<uint32_t>(_size.y) };
		xcb_configure_window(_connection, _window, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
	}

	void Frame::_onSetSize()
	{
		_wasResized = true;
	}

	Frame::Frame(const std::wstring& p_title, const spk::Vector2UInt& p_size, void* p_ptr) :
		spk::AbstractFrame(p_size)
	{
		_display = XOpenDisplay(NULL);
		if (_display == NULL)
			spk::throwException(L"Cannot open display");
		_defaultScreen = DefaultScreen(_display);

		_connection = XGetXCBConnection(_display);
		if (xcb_connection_has_error(_connection))
			spk::throwException(L"Cannot open connection to the X server");

		_screen = xcb_setup_roots_iterator(xcb_get_setup(_connection)).data;
		_window = xcb_generate_id(_connection);

		_createFrame();
		_nameFrame(p_title);

		_atom_wm_delete_window = _enableDestroyXCBProperty(_connection, _window);

		xcb_map_window(_connection, _window);
	}

	Frame::~Frame()
	{
		free(_atom_wm_delete_window);
		xcb_destroy_window(_connection, _window);
		xcb_disconnect(_connection);
	}

	bool Frame::wasResized() const
	{
		return _wasResized;
	};

	void Frame::resizeHandled()
	{
		_wasResized = false;
	}

	const spk::Vector2UInt& Frame::size() const
	{
		return (_size);
	}

	void Frame::render()
	{
		xcb_flush(_connection);
	}

	void Frame::clear()
	{
		xcb_clear_area(_connection, 0, _window, 0, 0, _size.x, _size.y);
	}
}