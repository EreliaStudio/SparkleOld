#pragma once

#include "application/modules/spk_abstract_module.hpp"

namespace spk
{
	class KeyboardModule : public IMessageConsumerModule
	{
	private:
		void _handleMessage(SystemMessage& p_event);

	public:
		KeyboardModule(spk::ThreadSafeQueue<SystemMessage> &p_queue);
		~KeyboardModule();

		void updateKeyboard();
	};
}