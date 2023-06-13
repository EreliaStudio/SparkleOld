#include "application/modules/spk_window_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "graphics/spk_window.hpp"

namespace spk
{
	void WindowModule::_handleMessage(SystemMessage& event)
	{
		unsigned int messageId;
		*event >> messageId;

		switch (messageId)
		{
		case WM_SIZE:
			unsigned int width;
			unsigned int height;

			*event >> width;
			*event >> height;

			spk::Window::instance()->resize(spk::Vector2Int(width, height));
			break;
		}
	}

	WindowModule::WindowModule(spk::ThreadSafeQueue<SystemMessage> &queue, const std::wstring& p_title, const spk::Vector2Int& p_size, void *p_apiModule) : IMessageConsumerModule(queue)
	{
		spk::Singleton<spk::Window>::instanciate(p_title, p_size, p_apiModule);
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