#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "design_pattern/spk_singleton.hpp"
#include "application/modules/spk_abstract_module.hpp"
#include "spk_system_define.hpp"

namespace spk
{
	class WindowModule : public IMessageConsumerModule
	{
	private:
		void _handleMessage(SystemMessage *event);

	public:
		WindowModule(spk::ThreadSafeQueue<SystemMessage *> &queue);

		void render();
		void clear();
	};
}