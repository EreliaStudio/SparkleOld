#include "application/modules/spk_widget_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "widget/spk_widget_atlas.hpp"
#include "spk_basic_functions.hpp"
#include "graphics/spk_window.hpp"
#include <cstring>

namespace spk
{
	WidgetModule::WidgetModule()
	{
		spk::Widget::Atlas::instanciate();
	}

	WidgetModule::~WidgetModule()
	{
		spk::Widget::Atlas::release();
	}

	void WidgetModule::update()
	{
		auto &widgetSet = spk::Widget::Atlas::instance()->widgets();

		for (auto it = widgetSet.rbegin(); it != widgetSet.rend(); ++it)
		{
			try
			{
				if ((*it)->_isOperationnal == true)
					(*it)->_update();
			}
			catch(const std::exception& e)
			{
				const char *tmp = e.what();
				std::wstring oldError = std::wstring(tmp, tmp + strlen(tmp));
				spk::throwException(L"Error during update [" + (*it)->name() + L"] : " + oldError);
			}
		}
		
	}

	void WidgetModule::render()
	{
		auto &widgetSet = spk::Widget::Atlas::instance()->widgets();

		for (auto it = widgetSet.begin(); it != widgetSet.end(); ++it)
		{
			if ((*it)->parent() != nullptr)
				(*it)->parent()->viewport().activate();
			else
				Viewport::resetViewport(spk::Window::instance()->size());
			if ((*it)->_isOperationnal == true)
				(*it)->_render();
		}
	}
}