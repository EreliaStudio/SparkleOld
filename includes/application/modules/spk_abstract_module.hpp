#pragma once

#include "threading/spk_thread_safe_queue.hpp"

#include "spk_system_define.hpp"

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
		spk::ThreadSafeQueue<SystemMessage> &_queue;

		virtual void _handleMessage(SystemMessage event) = 0;

	public:
		IMessageConsumerModule(spk::ThreadSafeQueue<SystemMessage> &queue);

		void update();
	};
}