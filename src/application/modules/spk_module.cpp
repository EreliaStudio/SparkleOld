#include "application/modules/spk_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "graphics/spk_window.hpp"
#include "widget/spk_widget_atlas.hpp"

namespace spk
{
	GraphicalAPIModule::GraphicalAPIModule(spk::ThreadSafeQueue<SystemMessage> &p_queue, const std::wstring& p_title, const spk::Vector2Int& p_size, void *p_apiModule) : IMessageConsumerModule(p_queue)
	{
		Window::instanciate(p_title, p_size, p_apiModule);
	}

	GraphicalAPIModule::~GraphicalAPIModule()
	{
		Window::release();
	}

	void GraphicalAPIModule::render()
	{
		Window::instance()->render();
	}

	void GraphicalAPIModule::clear()
	{
		Window::instance()->clear();
	}
}