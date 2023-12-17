#include "application/modules/spk_widget_module.hpp"
#include "application/spk_application.hpp"

namespace spk
{
	void WidgetModule::CentralWidget::_onGeometryChange()
	{

	}

	void WidgetModule::CentralWidget::_onRender()
	{

	}

	bool WidgetModule::CentralWidget::_onUpdate()
	{
		return (false);
	}

	WidgetModule::CentralWidget::CentralWidget() : 
		Widget(L"CentralWidget")
	{

	}

	WidgetModule::CentralWidget::~CentralWidget()
	{

	}


	WidgetModule::WidgetModule() :
		_centralWidget(nullptr)
	{
	}

	void WidgetModule::createCentralWidget()
	{
		_centralWidget = new CentralWidget();
		_centralWidget->setGeometry(0, spk::Application::instance()->size());
		_centralWidget->activate();
	}

	void WidgetModule::_applyWidgetResizeOperation(Widget* p_widget)
	{
		p_widget->_applyResizeOperation();
		for (size_t i = 0; i < p_widget->childrens().size(); i++)
		{
			_applyWidgetResizeOperation(p_widget->childrens()[i]);
		}
	}

	void WidgetModule::applyResizeOperation()
	{
		_applyWidgetResizeOperation(_centralWidget);
	}

	WidgetModule::CentralWidget* WidgetModule::centralWidget()
	{
		return (_centralWidget);
	}

	spk::Widget* WidgetModule::searchWidget(const std::wstring& p_name) const
	{
		return (_centralWidget->searchWidget(p_name));
	}

	void WidgetModule::renderWidgets()
	{
		_centralWidget->_render();
	}

	void WidgetModule::updateWidgets()
	{
		_centralWidget->_update();
	}
}