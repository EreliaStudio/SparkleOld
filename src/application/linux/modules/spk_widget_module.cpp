#include "application/linux/modules/spk_widget_module.hpp"

namespace spk
{
	void WidgetModule::CentralWidget::_onRender()
	{

	}
	bool WidgetModule::CentralWidget::_onUpdate()
	{
		return (false);
	}

	void WidgetModule::CentralWidget::_onGeometryChange()
	{

	}

	WidgetModule::CentralWidget::CentralWidget(std::wstring p_name) :
		AbstractWidget(p_name)
	{

	}

	WidgetModule::WidgetModule() :
		_centralWidget(new CentralWidget(L"CentralWidget"))
	{
		_centralWidget->activate();
	}

	WidgetModule::~WidgetModule()
	{
		delete _centralWidget;
	}
	
	void WidgetModule::update()
	{
		_centralWidget->_update();
	}

	void WidgetModule::render()
	{
	}

	AbstractWidget* WidgetModule::centralWidget() const
	{
		return (_centralWidget);
	}
}