#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "application/modules/spk_abstract_module.hpp"
#include "spk_system_define.hpp"

namespace spk
{
	class MouseModule : public IMessageConsumerModule
	{
	private:
		void _handleMessage(SystemMessage& event);

	public:
		MouseModule(ThreadSafeQueue<SystemMessage> &queue);
		~MouseModule();
		
		void updateMouse();
	};
}