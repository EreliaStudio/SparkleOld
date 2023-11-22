#pragma once

#include "application/modules/spk_message_consumer_module.hpp"

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
		void updateMouseDeltaPosition();
	};

}