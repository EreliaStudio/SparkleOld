#include "application/modules/spk_API_module.hpp"
#include "graphics/spk_window.hpp"
#include "iostream/spk_iostream.hpp"
#include "application/spk_application.hpp"

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

		while ((event = xcb_poll_for_event(Application::instance()->window().frame().connection())))
		{
			switch (event->response_type & ~0x80)
			{
			case XCB_CLIENT_MESSAGE:
				_systemQueue.push_back(std::move(event));
				break;
			case XCB_CONFIGURE_NOTIFY:
				_windowQueue.push_back(std::move(event));
				break;
			case XCB_ENTER_NOTIFY:
			case XCB_LEAVE_NOTIFY:
			case XCB_MOTION_NOTIFY:
			case XCB_BUTTON_RELEASE:
			case XCB_BUTTON_PRESS:
				_mouseQueue.push_back(std::move(event));
				break;
			case XCB_KEY_RELEASE:
			case XCB_KEY_PRESS:
				_keyboardQueue.push_back(std::move(event));
				break;
			default:
				spk::cout << L"APIModule::update() : Unhandled event {" << int(event->response_type & ~0x80) << L'}' << std::endl;
				break;
			}
		}
	}
}