#include "application/modules/spk_window_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "graphics/spk_window.hpp"
#include "widget/spk_widget_atlas.hpp"

namespace spk
{
	WindowModule::WindowModule(spk::ThreadSafeQueue<SystemMessage> &p_queue, const std::wstring& p_title, const spk::Vector2Int& p_size, const GraphicalAPI& p_graphicalAPI, void *p_apiModule) : IMessageConsumerModule(p_queue)
	{
		Window::instanciate(p_title, p_size, p_graphicalAPI, p_apiModule);
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