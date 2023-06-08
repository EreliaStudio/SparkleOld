#pragma once

#include "application/modules/spk_abstract_module.hpp"
#include "application/system/spk_window.hpp"
#include "iostream/spk_iostream.hpp"
#include "threading/spk_thread_safe_queue.hpp"
#include <xcb/xcb.h>

namespace spk
{

	class APIModule : public spk::AbstractModule
	{
	private:
		spk::ThreadSafeQueue<xcb_generic_event_t *> _windowQueue;
		spk::ThreadSafeQueue<xcb_generic_event_t *> _mouseQueue;
		spk::ThreadSafeQueue<xcb_generic_event_t *> _keyboardQueue;

	public:
		APIModule();
		~APIModule();

		void update();
		void render();

		spk::ThreadSafeQueue<xcb_generic_event_t *> &windowQueue() { return _windowQueue; }

		spk::ThreadSafeQueue<xcb_generic_event_t *> &mouseQueue() { return _mouseQueue; }

		spk::ThreadSafeQueue<xcb_generic_event_t *> &keyboardQueue() { return _keyboardQueue; }
	};
}