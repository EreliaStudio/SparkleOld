#include "application/modules/spk_keyboard_module.hpp"

namespace spk
{
	void KeyboardModule::_handleMessage(xcb_generic_event_t *event)
	{

	}

	KeyboardModule::KeyboardModule(spk::ThreadSafeQueue<xcb_generic_event_t *> &queue) : IMessageConsumerModule(queue)
	{
		
	}
}