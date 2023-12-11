#pragma once

#include "application/modules/spk_message_consumer_module.hpp"
#include "input/spk_keyboard.hpp"

namespace spk
{
	/**
	 * @class KeyboardModule
	 * @brief Class for managing the keyboard interactions.
	 * 
	 * This class manages the keyboard interactions, including handling system messages related to the keyboard.
	 */
	class KeyboardModule : public IMessageConsumerModule
	{
	private:
		spk::Keyboard _keyboard;

		/**
		 * @brief Handles a system message.
		 * 
		 * @param p_event The system message to handle.
		 */
		void _handleMessage(SystemMessage& p_event);

	public:
		/**
		 * @brief Constructs the KeyboardModule object.
		 * 
		 * @param p_queue The thread-safe queue for keyboard system messages.
		 */
		KeyboardModule(spk::ThreadSafeQueue<SystemMessage> &p_queue);

		/**
		 * @brief Destructs the KeyboardModule object.
		 */
		~KeyboardModule();

		/**
		 * @brief Updates the keyboard state.
		 */
		void updateKeyboard();
		
		void setKeyboardLayout(const spk::Keyboard::Layout& p_layout);

		spk::Keyboard& keyboard() {return (_keyboard);}
		const spk::Keyboard& keyboard() const {return (_keyboard);}
	};
}
