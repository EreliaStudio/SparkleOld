#pragma once

#include "application/modules/spk_abstract_module.hpp"
#include "application/spk_abstract_application.hpp"

namespace spk
{
	class SystemModule : public IMessageConsumerModule
	{
	private:
	
		spk::AbstractApplication* _applicationToManage;
		void _handleMessage(SystemMessage& event);

	public:
		SystemModule(ThreadSafeQueue<SystemMessage> &queue, spk::AbstractApplication* p_applicationToManage);
		~SystemModule();
		
		void updateMouse();
	};
}