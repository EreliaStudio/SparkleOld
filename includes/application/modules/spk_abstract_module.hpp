#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include <xcb/xcb.h>

namespace spk
{
	class AbstractModule
	{
	public:
		virtual void update() = 0;
	};

	class IMessageConsumerModule : public spk::AbstractModule
	{
	private:
		spk::ThreadSafeQueue<xcb_generic_event_t *> &_queue;

		virtual void _handleMessage(xcb_generic_event_t *event) = 0;

	public:
		IMessageConsumerModule(spk::ThreadSafeQueue<xcb_generic_event_t *> &queue);

		void update();
	};
}