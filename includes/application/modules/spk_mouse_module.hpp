#pragma once

#include "application/modules/spk_abstract_module.hpp"

namespace spk
{
	class MouseModule : public IMessageConsumerModule
	{
	private:
		void _handleMessage(SystemMessage& p_event);

	public:
		MouseModule(ThreadSafeQueue<SystemMessage> &p_queue);
		~MouseModule();
		
		void updateMouse();
	};
}