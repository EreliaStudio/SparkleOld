#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "application/modules/spk_abstract_module.hpp"
#include <xcb/xcb.h>

namespace spk
{
	class KeyboardModule : public IMessageConsumerModule
	{
	private:
		void _handleMessage(xcb_generic_event_t *event);

	public:
		KeyboardModule(spk::ThreadSafeQueue<xcb_generic_event_t *> &queue);
	};
}