#pragma once

#include "application/modules/spk_abstract_module.hpp"

namespace spk
{
	class KeyboardModule : public IMessageConsumerModule
	{
	private:
		void _handleMessage(SystemMessage& event);

	public:
		KeyboardModule(spk::ThreadSafeQueue<SystemMessage> &queue);
		~KeyboardModule();

		void updateKeyboard();
	};
}