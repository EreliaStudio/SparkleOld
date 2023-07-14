#include "application/modules/spk_keyboard_module.hpp"
#include "input/spk_keyboard.hpp"

namespace spk
{
	void KeyboardModule::_handleMessage(SystemMessage& p_event)
	{
		switch (p_event->response_type & ~0x80)
		{
			case XCB_KEY_PRESS:
			{
				xcb_key_press_event_t *keyPressEvent(reinterpret_cast<xcb_key_press_event_t *>(p_event));

				spk::Keyboard::instance()->pressKey(keyPressEvent->detail);
				break;
			}
			case XCB_KEY_RELEASE:
			{
				xcb_key_press_event_t *keyReleaseEvent(reinterpret_cast<xcb_key_press_event_t *>(p_event));

				spk::Keyboard::instance()->releaseKey(keyReleaseEvent->detail);
				break;
			}
		}
	}

	KeyboardModule::KeyboardModule(spk::ThreadSafeQueue<SystemMessage> &p_queue) : IMessageConsumerModule(p_queue)
	{
		spk::Keyboard::instanciate();
		spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Qwerty);
	}

	KeyboardModule::~KeyboardModule()
	{
		spk::Keyboard::release();
	}

	void KeyboardModule::updateKeyboard()
	{
		Singleton<Keyboard>::instance()->update();
	}
}