#include "application/modules/spk_API_module.hpp"
#include "graphics/spk_window.hpp"
#include "iostream/spk_iostream.hpp"

namespace spk
{	
	APIModule::APIModule()
	{

	}

	APIModule::~APIModule()
	{

	}

	void APIModule::pullMessage()
	{
		xcb_generic_event_t *event;
		auto window = spk::Window::instance();

		while ((event = xcb_poll_for_event(window->frame().connection())))
		{
			switch (event->response_type & ~0x80)
			{
			case XCB_CLIENT_MESSAGE:
				if(((xcb_client_message_event_t*)event)->data.data32[0] ==
					window->frame().atomWmDeleteWindow())
					event->response_type = XCB_DESTROY_NOTIFY;
				_systemQueue.push_back(event);
				break;
			case XCB_RESIZE_REQUEST:
				_windowQueue.push_back(event);
				break;
			case XCB_ENTER_NOTIFY:
			case XCB_LEAVE_NOTIFY:
			case XCB_MOTION_NOTIFY:
			case XCB_BUTTON_RELEASE:
			case XCB_BUTTON_PRESS:
				_mouseQueue.push_back(event);
				break;
			case XCB_KEY_RELEASE:
			case XCB_KEY_PRESS:
				_keyboardQueue.push_back(event);
				break;
			default:
				spk::cout << L"APIModule::update() : Unhandled event {" << int(event->response_type & ~0x80) << L'}' << std::endl;
				break;
			}
		}
	}
}