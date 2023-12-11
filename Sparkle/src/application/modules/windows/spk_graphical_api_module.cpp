#include "application/modules/spk_graphical_api_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "graphics/spk_window.hpp"
#include "widget/spk_widget_atlas.hpp"

namespace spk
{
	void GraphicalAPIModule::_handleMessage(SystemMessage& p_event)
	{
		unsigned int messageId;
		*p_event >> messageId;

		switch (messageId)
		{
		case WM_SIZE:
			unsigned int width;
			unsigned int height;

			*p_event >> width;
			*p_event >> height;

			Window::instance()->resize(spk::Vector2Int(width, height));
			Widget::Atlas::instance()->resize();

			break;
		case WM_MOVE:
		{
			unsigned int x;
			unsigned int y;

			*p_event >> x;
			*p_event >> y;

			Window::instance()->place(spk::Vector2Int(x, y));
			break;
		}
		}
	}
}