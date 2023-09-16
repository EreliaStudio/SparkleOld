#include "application/modules/spk_graphical_api_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "graphics/spk_window.hpp"
#include "widget/spk_widget_atlas.hpp"

namespace spk
{
	void GraphicalAPIModule::_handleMessage(SystemMessage& p_event)
	{
		switch (p_event->response_type & ~0x80)
		{
			case XCB_RESIZE_REQUEST:
			{
				xcb_resize_request_event_t *resizeRequestEvent = reinterpret_cast<xcb_resize_request_event_t *>(p_event);

				spk::Vector2 ratio = spk::Vector2(
					static_cast<float>(resizeRequestEvent->width) / Window::instance()->size().x,
					static_cast<float>(resizeRequestEvent->height) / Window::instance()->size().y
				);

				Window::instance()->resize(spk::Vector2Int(resizeRequestEvent->width, resizeRequestEvent->height));
				Widget::Atlas::instance()->resize(ratio);
				break;
			}
		}
	}
}