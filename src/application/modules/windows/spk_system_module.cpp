#include "application/modules/spk_system_module.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
	void SystemModule::_handleMessage(SystemMessage& event)
	{
		DEBUG_LINE();

		unsigned int messageId;
		*event >> messageId;

		switch (messageId)
		{
		case WM_DESTROY:
		{
			_applicationToManage->quit(0);
			break;
		}
		}
	}

	SystemModule::SystemModule(ThreadSafeQueue<SystemMessage> &queue, spk::AbstractApplication* p_applicationToManage) : IMessageConsumerModule(queue),
		_applicationToManage(p_applicationToManage)
	{

	}

	SystemModule::~SystemModule()
	{

	}

	void SystemModule::updateMouse()
	{
		
	}
}