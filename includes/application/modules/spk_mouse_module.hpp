#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "application/modules/spk_abstract_module.hpp"
#include <xcb/xcb.h>
#include "application/system/spk_mouse.hpp"

namespace spk
{
	class MouseModule : public IMessageConsumerModule
	{
	private:
		void _handleMessage(xcb_generic_event_t *event)
		{
			switch (event->response_type & ~0x80)
			{
			case XCB_MOTION_NOTIFY:
			{
				xcb_motion_notify_event_t *motionNotifyEvent(reinterpret_cast<xcb_motion_notify_event_t *>(event));

				Singleton<Mouse>::instance()->setMousePosition(Vector2Int(motionNotifyEvent->event_x, motionNotifyEvent->event_y));

				break;
			}
			case XCB_BUTTON_PRESS:
			{
				xcb_button_press_event_t *buttonPressEvent(reinterpret_cast<xcb_button_press_event_t *>(event));

				Singleton<Mouse>::instance()->pressButton(static_cast<Mouse::Button>(buttonPressEvent->detail));

				break;
			}
			case XCB_BUTTON_RELEASE:
			{
				xcb_button_release_event_t *buttonReleaseEvent(reinterpret_cast<xcb_button_release_event_t *>(event));

				Singleton<Mouse>::instance()->releaseButton(static_cast<Mouse::Button>(buttonReleaseEvent->detail));

				break;
			}
			}
		}

	public:
		MouseModule(ThreadSafeQueue<xcb_generic_event_t *> &queue) : IMessageConsumerModule(queue)
		{
		}

		void updateMouse()
		{
			Singleton<Mouse>::instance()->update();
		}
	};
}