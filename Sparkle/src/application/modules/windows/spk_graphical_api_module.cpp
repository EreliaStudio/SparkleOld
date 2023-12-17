#include "application/modules/spk_graphical_api_module.hpp"
#include "iostream/spk_iostream.hpp"
#include "graphics/spk_window.hpp"
#include "application/spk_application.hpp"

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


			resize(spk::Vector2Int(width, height));

			break;
		case WM_MOVE:
		{
			unsigned int x;
			unsigned int y;

			*p_event >> x;
			*p_event >> y;

			_window.place(spk::Vector2Int(x, y));
			break;
		}
		}
	}
}