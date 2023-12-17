#include "application/modules/spk_mouse_module.hpp"
#include "input/spk_mouse.hpp"

namespace spk
{
	void MouseModule::_handleMessage(SystemMessage& p_event)
	{
		unsigned int messageId;
		*p_event >> messageId;

		switch (messageId)
		{
		case WM_LBUTTONDOWN:
		{
			_mouse.pressButton(Mouse::Left);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			_mouse.pressButton(Mouse::Middle);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			_mouse.pressButton(Mouse::Right);
			break;
		}
		case WM_LBUTTONUP:
		{
			_mouse.releaseButton(Mouse::Left);

			break;
		}
		case WM_MBUTTONUP:
		{
			_mouse.releaseButton(Mouse::Middle);

			break;
		}
		case WM_RBUTTONUP:
		{
			_mouse.releaseButton(Mouse::Right);

			break;
		}
		case WM_XBUTTONDOWN :
		{
			short value;

			*p_event >> value;

			if (value == MK_XBUTTON1)
			{
				_mouse.pressButton(Mouse::Button3);
			}
			else
			{
				_mouse.pressButton(Mouse::Button4);
			}
			break;
		}
		case WM_XBUTTONUP :
		{
			short value;

			*p_event >> value;

			if (value == MK_XBUTTON1)
			{
				_mouse.releaseButton(Mouse::Button3);
			}
			else
			{
				_mouse.releaseButton(Mouse::Button4);
			}

			break;
		}
		case WM_MOUSEWHEEL:
		{
			short value;

			*p_event >> value;

			_mouse.editWheelPosition(Vector2Int(0, (int)value));

			break;
		}
		case WM_MOUSEHWHEEL:
		{
			short value;

			*p_event >> value;
			
			_mouse.editWheelPosition(Vector2Int((int)value, 0));

			break;
		}
		case WM_MOUSEMOVE:
		{
			int newPosX, newPosY;
		
			*p_event >> newPosX;
			*p_event >> newPosY;
		
			_mouse.setMousePosition(Vector2Int(newPosX, newPosY));

			break;
		}
		}
	}

	MouseModule::MouseModule(ThreadSafeQueue<SystemMessage> &p_queue) :
		IMessageConsumerModule(p_queue)
	{
		
	}

	MouseModule::~MouseModule()
	{

	}

	void MouseModule::updateMouse()
	{
		_mouse.update();
	}
}