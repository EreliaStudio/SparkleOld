#include "application/modules/spk_window_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "graphics/spk_window.hpp"

namespace spk
{
	void WindowModule::_handleMessage(SystemMessage& event)
	{

	}

	WindowModule::WindowModule(spk::ThreadSafeQueue<SystemMessage> &queue, spk::Vector2Int p_size, void* p_apiModule) : IMessageConsumerModule(queue)
	{
		spk::Singleton<spk::Window>::instanciate(p_size, p_apiModule);
	}

	void WindowModule::render()
	{
		spk::Singleton<Window>::instance()->render();
	}

	void WindowModule::clear()
	{
		spk::Singleton<Window>::instance()->clear();
	}
}