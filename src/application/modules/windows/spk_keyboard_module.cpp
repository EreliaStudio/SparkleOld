#include "application/modules/spk_keyboard_module.hpp"
#include "input/spk_keyboard.hpp"

namespace spk
{
	void KeyboardModule::_handleMessage(SystemMessage& event)
	{
		
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