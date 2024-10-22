#include "application/modules/spk_mouse_module.hpp"
#include "input/spk_mouse.hpp"

namespace spk
{
	void MouseModule::_handleMessage(SystemMessage& p_event)
	{
		switch (p_event->response_type & ~0x80)
		{
		case XCB_MOTION_NOTIFY:
		{
			xcb_motion_notify_event_t *motionNotifyEvent(reinterpret_cast<xcb_motion_notify_event_t *>(p_event));

			Singleton<Mouse>::instance()->setMousePosition(Vector2Int(motionNotifyEvent->event_x, motionNotifyEvent->event_y));

			break;
		}
		case XCB_BUTTON_PRESS:
		{
			xcb_button_press_event_t *buttonPressEvent(reinterpret_cast<xcb_button_press_event_t *>(p_event));

			switch (buttonPressEvent->detail)
			{
				case 1:
					Singleton<Mouse>::instance()->pressButton(Mouse::Left);
					break;
				case 2:
					Singleton<Mouse>::instance()->pressButton(Mouse::Middle);
					break;
				case 3:
					Singleton<Mouse>::instance()->pressButton(Mouse::Right);
					break;
				case 4:
					Singleton<Mouse>::instance()->editWheelPosition(Vector2Int(0, 1));
					break;
				case 5:
					Singleton<Mouse>::instance()->editWheelPosition(Vector2Int(0, -1));
					break;
				case 6:
					Singleton<Mouse>::instance()->editWheelPosition(Vector2Int(-1, 0));
					break;
				case 7:
					Singleton<Mouse>::instance()->editWheelPosition(Vector2Int(1, 0));
					break;
				case 8:
					Singleton<Mouse>::instance()->pressButton(Mouse::Button3);
					break;
				case 9:
					Singleton<Mouse>::instance()->pressButton(Mouse::Button4);
					break;
			}

			break;
		}
		case XCB_BUTTON_RELEASE:
		{
			xcb_button_release_event_t *buttonReleaseEvent(reinterpret_cast<xcb_button_release_event_t *>(p_event));

			switch (buttonReleaseEvent->detail)
			{
				case 1:
					Singleton<Mouse>::instance()->releaseButton(Mouse::Left);
					break;
				case 2:
					Singleton<Mouse>::instance()->releaseButton(Mouse::Middle);
					break;
				case 3:
					Singleton<Mouse>::instance()->releaseButton(Mouse::Right);
					break;
				case 8:
					Singleton<Mouse>::instance()->releaseButton(Mouse::Button3);
					break;
				case 9:
					Singleton<Mouse>::instance()->releaseButton(Mouse::Button4);
					break;
			}

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