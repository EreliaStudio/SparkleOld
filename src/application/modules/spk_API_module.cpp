#include "application/modules/spk_API_module.hpp"

namespace spk
{
#ifdef _WIN32
	LRESULT CALLBACK APIModule::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

	}
#endif
	
	APIModule::APIModule()
	{

	}

	APIModule::~APIModule()
	{

	}

	void APIModule::update()
	{
#ifdef _WIN32
			MSG msg = {};
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				int translateResult = TranslateMessage(&msg);
				int dispatchResult = static_cast<int>(DispatchMessage(&msg));
			}
#elif __linux__
		xcb_generic_event_t *event;

		while (event = xcb_poll_for_event(spk::Singleton<Window>::instance()->_connection))
		{
			switch (event->response_type & ~0x80)
			{
			case XCB_RESIZE_REQUEST:
				_windowQueue.push_back(event);
				break;
			case XCB_ENTER_NOTIFY:
			case XCB_LEAVE_NOTIFY:
			case XCB_MOTION_NOTIFY:
			// case XCB_EVENT_MASK_BUTTON_RELEASE: case XCB_EVENT_MASK_BUTTON_PRESS:
			case XCB_BUTTON_RELEASE:
			case XCB_BUTTON_PRESS:
				_mouseQueue.push_back(event);
				break;
			case XCB_KEY_RELEASE:
			case XCB_KEY_PRESS:
				// case XCB_EVENT_MASK_KEY_RELEASE: case XCB_EVENT_MASK_KEY_PRESS:
				_keyboardQueue.push_back(event);
				break;
			default:
				spk::cout << L"APIModule::update() : Unhandled event {" << std::hex << int(event->response_type & ~0x80) << L'}' << std::endl;
				break;
			}
		}
#endif

	}

	void APIModule::render()
	{
		
	}
}