#include "application/modules/spk_window_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "graphics/spk_window.hpp"

namespace spk
{
	void WindowModule::_handleMessage(SystemMessage& p_event)
	{
		switch (p_event->response_type & ~0x80)
		{
			case XCB_RESIZE_REQUEST:
			{
				xcb_resize_request_event_t *resizeRequestEvent = reinterpret_cast<xcb_resize_request_event_t *>(p_event);
				
				spk::Singleton<Window>::instance()->resize(spk::Vector2Int(resizeRequestEvent->width, resizeRequestEvent->height));
				break;
			}
		}
	}

	WindowModule::WindowModule(spk::ThreadSafeQueue<SystemMessage> &p_queue, const std::wstring& p_title, const spk::Vector2Int& p_size) : IMessageConsumerModule(p_queue)
	{
		spk::Singleton<spk::Window>::instanciate(p_title, p_size);
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