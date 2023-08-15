#include "application/modules/spk_window_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "graphics/spk_window.hpp"
#include "widget/spk_widget_atlas.hpp"

namespace spk
{
	void WindowModule::_handleMessage(SystemMessage& p_event)
	{
		unsigned int messageId;
		*p_event >> messageId;

		switch (messageId)
		{
		case WM_SIZE:
			unsigned int width;
			unsigned int height;

			*p_event >> width;
			*p_event >> height;

			spk::Vector2 ratio = spk::Vector2(
				static_cast<float>(width) / Window::instance()->size().x,
				static_cast<float>(height) / Window::instance()->size().y
			);

			Window::instance()->resize(spk::Vector2Int(width, height));
			Widget::Atlas::instance()->resize(ratio);

			break;
		}
	}

	WindowModule::WindowModule(spk::ThreadSafeQueue<SystemMessage> &p_queue, const std::wstring& p_title, const spk::Vector2Int& p_size, void *p_apiModule) : IMessageConsumerModule(p_queue)
	{
		Window::instanciate(p_title, p_size, p_apiModule);
	}

	WindowModule::~WindowModule()
	{
		Window::release();
	}

	void WindowModule::render()
	{
		Window::instance()->render();
	}

	void WindowModule::clear()
	{
		Window::instance()->clear();
	}
}