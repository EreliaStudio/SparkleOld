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
		auto &widgetSet = spk::WidgetAtlas::instance()->widgets();

		for (auto it = widgetSet.rbegin(); it != widgetSet.rend(); ++it)
		{
			if ((*it)->_isOperationnal == true)
				(*it)->_update();
		}
	}

	void WidgetModule::render()
	{
		auto &widgetSet = spk::WidgetAtlas::instance()->widgets();

		for (auto it = widgetSet.begin(); it != widgetSet.end(); ++it)
		{
			if ((*it)->_isOperationnal == true)
				(*it)->_render();
		}
	}
}