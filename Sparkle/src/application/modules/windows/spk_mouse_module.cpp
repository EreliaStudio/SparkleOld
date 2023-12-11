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

			*p_event >> value;

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

			*p_event >> value;

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

			*p_event >> value;

			Singleton<Mouse>::instance()->editWheelPosition(Vector2Int(0, (int)value));

			break;
		}
		case WM_MOUSEHWHEEL:
		{
			short value;

			*p_event >> value;
			
			Singleton<Mouse>::instance()->editWheelPosition(Vector2Int((int)value, 0));

			break;
		}
		case WM_MOUSEMOVE:
		{
			int newPosX, newPosY;
		
			*p_event >> newPosX;
			*p_event >> newPosY;
		
			Singleton<Mouse>::instance()->setMousePosition(Vector2Int(newPosX, newPosY));

			break;
		}
		}
	}

	MouseModule::MouseModule(ThreadSafeQueue<SystemMessage> &p_queue) : IMessageConsumerModule(p_queue)
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