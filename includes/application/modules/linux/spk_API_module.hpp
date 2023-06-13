#pragma once

#include "application/modules/spk_abstract_module.hpp"
#include "application/graphics/spk_window.hpp"
#include "iostream/spk_iostream.hpp"
#include "threading/spk_thread_safe_queue.hpp"

namespace spk
{

	class APIModule : public spk::AbstractModule
	{
	private:
		spk::ThreadSafeQueue<SystemMessage> _windowQueue;
		spk::ThreadSafeQueue<SystemMessage> _mouseQueue;
		spk::ThreadSafeQueue<SystemMessage> _keyboardQueue;

	public:
		APIModule();
		~APIModule();

		void update();
		void render();

		spk::ThreadSafeQueue<SystemMessage> &windowQueue() { return _windowQueue; }

		spk::ThreadSafeQueue<SystemMessage> &mouseQueue() { return _mouseQueue; }

		spk::ThreadSafeQueue<SystemMessage> &keyboardQueue() { return _keyboardQueue; }
	};
}