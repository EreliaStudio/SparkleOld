#include "application/modules/spk_keyboard_module.hpp"
#include "input/spk_keyboard.hpp"

namespace spk
{
	void KeyboardModule::_handleMessage(SystemMessage& p_event)
	{
		unsigned int messageId;
		*p_event >> messageId;

		switch (messageId)
		{
		case WM_CHAR:
		{
			unsigned int value;

			*p_event >> value;

			break;
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			unsigned int value;

			*p_event >> value;

			spk::Keyboard::instance()->pressKey(value);

			break;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			unsigned int value;

			*p_event >> value;

			spk::Keyboard::instance()->releaseKey(value);

			break;
		}
		}
	}

	KeyboardModule::KeyboardModule(spk::ThreadSafeQueue<SystemMessage> &p_queue) : IMessageConsumerModule(p_queue)
	{
		spk::Keyboard::instanciate();
		spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);
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