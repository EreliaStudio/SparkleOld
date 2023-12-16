#include "application/modules/spk_graphical_api_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "graphics/spk_window.hpp"
#include "widget/spk_widget_atlas.hpp"

namespace spk
{
	GraphicalAPIModule::GraphicalAPIModule(spk::ThreadSafeQueue<SystemMessage> &p_queue, const std::wstring& p_title, const spk::Vector2Int& p_size, void *p_apiModule) :
		IMessageConsumerModule(p_queue),
		_window(p_title, p_size, p_apiModule)
	{

	}

	GraphicalAPIModule::~GraphicalAPIModule()
	{

	}

	void GraphicalAPIModule::render()
	{
		_window.render();
	}

	void GraphicalAPIModule::clear()
	{
		_window.clear();
	}

	void GraphicalAPIModule::resize(const spk::Vector2UInt& p_newSize)
	{
		_window.resize(p_newSize);
	}
}