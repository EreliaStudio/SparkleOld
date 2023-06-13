#include "application/modules/spk_keyboard_module.hpp"
#include "input/spk_keyboard.hpp"

namespace spk
{
	void KeyboardModule::_handleMessage(SystemMessage& event)
	{
		unsigned int messageId;
		*event >> messageId;

		switch (messageId)
		{
		case WM_CHAR:
		{
			unsigned int value;

			*event >> value;

			break;
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			unsigned int value;

			*event >> value;

			spk::Singleton<spk::Keyboard>::instance()->pressKey(value);

			break;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			unsigned int value;

			*event >> value;

			spk::Singleton<spk::Keyboard>::instance()->releaseKey(value);

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