#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "application/linux/modules/spk_abstract_module.hpp"
#include <xcb/xcb.h>
#include "application/linux/input/spk_mouse.hpp"

namespace spk
{
	class MouseModule : public IMessageConsumerModule
	{
	private:
		void _handleMessage(xcb_generic_event_t *event);

	public:
		MouseModule(ThreadSafeQueue<xcb_generic_event_t *> &queue);
		~MouseModule();
		
		void updateMouse();
	};
}