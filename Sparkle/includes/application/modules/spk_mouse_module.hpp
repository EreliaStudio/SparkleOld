#pragma once

#include "application/modules/spk_message_consumer_module.hpp"
#include "input/spk_mouse.hpp"

namespace spk
{
	/**
	 * @class MouseModule
	 * @brief Class for managing the mouse interactions.
	 *
	 * This class manages the mouse interactions, including handling system messages related to the mouse.
	 */
	class MouseModule : public IMessageConsumerModule
	{
	private:
		spk::Mouse _mouse;

		/**
		 * @brief Handles a system message.
		 *
		 * @param p_event The system message to handle.
		 */
		void _handleMessage(SystemMessage &p_event);

	public:
		/**
		 * @brief Constructs the MouseModule object.
		 *
		 * @param p_queue The thread-safe queue for mouse system messages.
		 */
		MouseModule(ThreadSafeQueue<SystemMessage> &p_queue);

		/**
		 * @brief Destructs the MouseModule object.
		 */
		~MouseModule();

		/**
		 * @brief Updates the mouse state.
		 */
		void updateMouse();

		/**
		 * @brief Updates the mouse delta position.
		 */
		void updateMouseDeltaPosition();
		
		/**
		 * @brief Provides non-const access to the mouse instance.
		 * 
		 * @return Reference to the mouse instance.
		 */
		spk::Mouse& mouse() { return (_mouse); }

		/**
		 * @brief Provides const access to the mouse instance.
		 * 
		 * @return Const reference to the mouse instance.
		 */
		const spk::Mouse& mouse() const { return (_mouse); }
	};

}