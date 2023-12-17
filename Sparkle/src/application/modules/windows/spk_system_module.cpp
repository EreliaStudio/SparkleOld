#include "application/modules/spk_system_module.hpp"
#include "application/spk_application.hpp"
#include "iostream/spk_iostream.hpp"


namespace spk
{
	void SystemModule::_handleMessage(SystemMessage& p_event)
	{
		unsigned int messageId;
		*p_event >> messageId;

		switch (messageId)
		{
		case WM_DESTROY:
		{
			_applicationToManage->quit(0);
			break;
		}
		}
	}

	SystemModule::SystemModule(ThreadSafeQueue<SystemMessage> &p_queue, spk::Application* p_applicationToManage) :
		IMessageConsumerModule(p_queue),
		_applicationToManage(p_applicationToManage)
	{
		
	}

	SystemModule::~SystemModule()
	{

	}
}