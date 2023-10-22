#include "application/modules/spk_system_module.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
	void SystemModule::_handleMessage(SystemMessage& p_event)
	{
		if (p_event->response_type == XCB_DESTROY_NOTIFY)
		{
			_applicationToManage->quit(0);
		}
	}

	SystemModule::SystemModule(ThreadSafeQueue<SystemMessage> &p_queue, std::shared_ptr<spk::AbstractApplication> p_applicationToManage) : IMessageConsumerModule(p_queue),
		_applicationToManage(p_applicationToManage)
	{

	}

	SystemModule::~SystemModule()
	{

	}
}