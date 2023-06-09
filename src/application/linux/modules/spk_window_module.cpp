#include "application/linux/modules/spk_window_module.hpp"
#include "iostream/spk_iostream.hpp"
namespace spk
{
	void WindowModule::_handleMessage(xcb_generic_event_t *event)
	{
		switch (event->response_type & ~0x80)
		{
			case XCB_RESIZE_REQUEST:
			{
				xcb_resize_request_event_t *resizeRequestEvent = reinterpret_cast<xcb_resize_request_event_t *>(event);
				
				spk::Singleton<Window>::instance()->resize(spk::Vector2Int(resizeRequestEvent->width, resizeRequestEvent->height));
				break;
			}
		}
	}

	WindowModule::WindowModule(spk::ThreadSafeQueue<xcb_generic_event_t *> &queue) : IMessageConsumerModule(queue)
	{

	}

	void WindowModule::render()
	{
		spk::Singleton<Window>::instance()->render();
	}

	void WindowModule::clear()
	{
		spk::Singleton<Window>::instance()->clear();
	}
}