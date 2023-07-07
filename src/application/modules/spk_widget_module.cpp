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

	}

	void WidgetModule::render()
	{

	}
}