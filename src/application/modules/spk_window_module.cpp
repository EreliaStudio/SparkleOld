#include "application/modules/spk_window_module.hpp"

namespace spk
{
	void WindowModule::_handleMessage(xcb_generic_event_t *event)
	{

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