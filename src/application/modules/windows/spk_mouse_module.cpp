#include "application/modules/spk_mouse_module.hpp"
#include "input/spk_mouse.hpp"

namespace spk
{
	void MouseModule::_handleMessage(SystemMessage& event)
	{
		
	}

	MouseModule::MouseModule(ThreadSafeQueue<SystemMessage> &queue) : IMessageConsumerModule(queue)
	{
		spk::Singleton<spk::Mouse>::instanciate();
	}

	MouseModule::~MouseModule()
	{
		spk::Singleton<spk::Mouse>::release();
	}

	void MouseModule::updateMouse()
	{
		Singleton<Mouse>::instance()->update();
	}
}