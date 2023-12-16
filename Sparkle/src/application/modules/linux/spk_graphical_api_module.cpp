#include "application/modules/spk_graphical_api_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "graphics/spk_window.hpp"
#include "widget/spk_widget_atlas.hpp"
#include "application/spk_application.hpp"

namespace spk
{
	void GraphicalAPIModule::_handleMessage(SystemMessage& p_event)
	{

		switch (p_event->response_type & ~0x80)
		{
		case XCB_CONFIGURE_NOTIFY:
		{
			xcb_configure_notify_event_t* configureNotifyEvent = reinterpret_cast<xcb_configure_notify_event_t*>(p_event);

			if (_window.size().x != configureNotifyEvent->width || _window.size().y != configureNotifyEvent->height)
			{
				_window.setSize(spk::Vector2Int(configureNotifyEvent->width, configureNotifyEvent->height));
				Widget::Atlas::instance()->resize();
			}
			break;
		}
		}
	}
}