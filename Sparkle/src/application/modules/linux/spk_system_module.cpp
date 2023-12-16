#include "application/modules/spk_system_module.hpp"
#include "application/spk_application.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
	void SystemModule::_handleMessage(SystemMessage& p_event)
	{
		if (((xcb_client_message_event_t*)p_event)->data.data32[0] ==
			_applicationToManage->window().frame().atomWmDeleteWindow())
		{
			_applicationToManage->quit(0);
		}
	}

	SystemModule::SystemModule(ThreadSafeQueue<SystemMessage>& p_queue, spk::Application* p_applicationToManage) :
		IMessageConsumerModule(p_queue),
		_applicationToManage(p_applicationToManage)
	{

	}

	SystemModule::~SystemModule()
	{

	}
}