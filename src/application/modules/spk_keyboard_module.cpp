#include "application/modules/spk_keyboard_module.hpp"

namespace spk
{
	void KeyboardModule::_handleMessage(xcb_generic_event_t *event)
	{
		switch (event->response_type & ~0x80)
		{
			case XCB_KEY_PRESS:
			{
				xcb_key_press_event_t *keyPressEvent(reinterpret_cast<xcb_key_press_event_t *>(event));

				spk::Singleton<spk::Keyboard>::instance()->pressKey(static_cast<spk::Keyboard::Key>(keyPressEvent->detail));
				break;
			}
			case XCB_KEY_RELEASE:
			{
				xcb_key_press_event_t *keyReleaseEvent(reinterpret_cast<xcb_key_press_event_t *>(event));

				spk::Singleton<spk::Keyboard>::instance()->releaseKey(static_cast<spk::Keyboard::Key>(keyReleaseEvent->detail));
				break;
			}
		}
	}

	KeyboardModule::KeyboardModule(spk::ThreadSafeQueue<xcb_generic_event_t *> &queue) : IMessageConsumerModule(queue)
	{
		spk::Singleton<spk::Keyboard>::instanciate();
	}

	KeyboardModule::~KeyboardModule()
	{
		spk::Singleton<spk::Keyboard>::release();
	}

	void KeyboardModule::updateKeyboard()
	{
		Singleton<Keyboard>::instance()->update();
	}
}