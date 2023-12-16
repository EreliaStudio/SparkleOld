#include "application/modules/spk_widget_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "widget/spk_widget_atlas.hpp"
#include "spk_basic_functions.hpp"
#include "application/spk_application.hpp"
#include <cstring>
#include <ranges>

namespace spk
{
	WidgetModule::WidgetModule()
	{
		spk::Widget::Atlas::instanciate();
		_widgetSet = &(Widget::Atlas::instance()->widgets());
	}

	WidgetModule::~WidgetModule()
	{
		spk::Widget::Atlas::release();
	}

	void WidgetModule::update()
	{
		for (const auto& widget : *_widgetSet)
		{
			if (widget->_isOperationnal == true && widget->_update() == true)
				return ;
		}
	}

	void WidgetModule::render()
	{
		for (auto& widget : *_widgetSet | std::views::reverse)
		{
			if (widget->parent() != nullptr)
			{
				if (widget->parent()->viewport().needComputation() == true)
					widget->parent()->viewport().compute();
				widget->parent()->viewport().activate();
			}
			else
				Viewport::resetViewport(spk::Application::instance()->window().size());
			if (widget->_isOperationnal == true)
				widget->_render();
		}
	}
}