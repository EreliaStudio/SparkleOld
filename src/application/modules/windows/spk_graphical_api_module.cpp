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

			spk::Vector2 ratio = spk::Vector2(
				static_cast<float>(width) / static_cast<float>(Window::instance()->size().x),
				static_cast<float>(height) / static_cast<float>(Window::instance()->size().y)
			);

			Window::instance()->resize(spk::Vector2Int(width, height));
			Widget::Atlas::instance()->resize(ratio);

			break;
		}
	}
}