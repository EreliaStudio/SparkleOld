#include "application/modules/spk_keyboard_module.hpp"
#include "input/spk_keyboard.hpp"

namespace spk
{
	void KeyboardModule::_handleMessage(SystemMessage& event)
	{
		switch (event->response_type & ~0x80)
		{
			case XCB_KEY_PRESS:
			{
				xcb_key_press_event_t *keyPressEvent(reinterpret_cast<xcb_key_press_event_t *>(event));

				spk::Singleton<spk::Keyboard>::instance()->pressKey(keyPressEvent->detail);
				break;
			}
			case XCB_KEY_RELEASE:
			{
				xcb_key_press_event_t *keyReleaseEvent(reinterpret_cast<xcb_key_press_event_t *>(event));

				spk::Singleton<spk::Keyboard>::instance()->releaseKey(keyReleaseEvent->detail);
				break;
			}
		}
	}

	KeyboardModule::KeyboardModule(spk::ThreadSafeQueue<SystemMessage> &queue) : IMessageConsumerModule(queue)
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