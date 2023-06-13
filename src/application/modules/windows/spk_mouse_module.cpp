#include "application/modules/spk_mouse_module.hpp"
#include "input/spk_mouse.hpp"

namespace spk
{
	void MouseModule::_handleMessage(SystemMessage& event)
	{
		unsigned int messageId;
		*event >> messageId;

		switch (messageId)
		{
		case WM_LBUTTONDOWN:
		{
			Singleton<Mouse>::instance()->pressButton(Mouse::Left);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			Singleton<Mouse>::instance()->pressButton(Mouse::Middle);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			Singleton<Mouse>::instance()->pressButton(Mouse::Right);
			break;
		}
		case WM_LBUTTONUP:
		{
			Singleton<Mouse>::instance()->releaseButton(Mouse::Left);

			break;
		}
		case WM_MBUTTONUP:
		{
			Singleton<Mouse>::instance()->releaseButton(Mouse::Middle);

			break;
		}
		case WM_RBUTTONUP:
		{
			Singleton<Mouse>::instance()->releaseButton(Mouse::Right);

			break;
		}
		case WM_XBUTTONDOWN :
		{
			short value;

			*event >> value;

			if (value == MK_XBUTTON1)
			{
				Singleton<Mouse>::instance()->pressButton(Mouse::Button3);
			}
			else
			{
				Singleton<Mouse>::instance()->pressButton(Mouse::Button4);
			}
			break;
		}
		case WM_XBUTTONUP :
		{
			short value;

			*event >> value;

			if (value == MK_XBUTTON1)
			{
				Singleton<Mouse>::instance()->releaseButton(Mouse::Button3);
			}
			else
			{
				Singleton<Mouse>::instance()->releaseButton(Mouse::Button4);
			}

			break;
		}
		case WM_MOUSEWHEEL:
		{
			short value;

			*event >> value;

			Singleton<Mouse>::instance()->editWheelPosition(Vector2Int(0, (int)value));

			break;
		}
		case WM_MOUSEHWHEEL:
		{
			short value;

			*event >> value;
			
			Singleton<Mouse>::instance()->editWheelPosition(Vector2Int((int)value, 0));

			break;
		}
		case WM_MOUSEMOVE:
		{
			int newPosX, newPosY;
			*event >> newPosX;
			*event >> newPosY;

			Singleton<Mouse>::instance()->setMousePosition(Vector2Int(newPosX, newPosY));

			break;
		}
		}
	}

	MouseModule::MouseModule(ThreadSafeQueue<SystemMessage> &queue) : IMessageConsumerModule(queue)
	{
		spk::Singleton<spk::Mouse>::instanciate();
	}

	MouseModule::~MouseModule()
	{
		spk::Singleton<spk::Mouse>::release();
	}

	void MouseModule::updateMouse()
	{
		Singleton<Mouse>::instance()->update();
	}
}