#pragma once

#include "application/modules/spk_abstract_module.hpp"

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