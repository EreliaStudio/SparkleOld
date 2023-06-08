#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "design_pattern/spk_singleton.hpp"
#include "application/modules/spk_abstract_module.hpp"
#include "application/system/spk_window.hpp"
#include <xcb/xcb.h>

namespace spk
{
	class WindowModule : public IMessageConsumerModule
	{
	private:
		void _handleMessage(xcb_generic_event_t *event);

	public:
		WindowModule(spk::ThreadSafeQueue<xcb_generic_event_t *> &queue);

		void render();
		void clear();
	};
}