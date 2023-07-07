#include "application/modules/spk_widget_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "widget/spk_widget_atlas.hpp"

namespace spk
{
	WidgetModule::WidgetModule()
	{
		spk::WidgetAtlas::instanciate();
	}

	WidgetModule::~WidgetModule()
	{
		spk::WidgetAtlas::release();
	}

	void WidgetModule::update()
	{
		auto &widgetSet = spk::WidgetAtlas::instance()->getWidgetSet();

		for (auto it = widgetSet.rbegin(); it != widgetSet.rend(); ++it)
		{
			(*it)->_update();
		}
	}

	void WidgetModule::render()
	{
		auto &widgetSet = spk::WidgetAtlas::instance()->getWidgetSet();

		for (auto &widget : widgetSet)
		{
			widget->_render();
		}
	}
}