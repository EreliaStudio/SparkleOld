#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "spk_system_define.hpp"

namespace spk
{
	/**
	 * @class IMessageConsumerModule
	 * @brief Class for modules that consume messages.
	 * 
	 * This class provides an interface for modules that need to consume system messages.
	 */
	class IMessageConsumerModule
	{
	private:
		spk::ThreadSafeQueue<SystemMessage> &_queue; ///< Thread-safe queue for system messages.

		/**
		 * @brief Handles a system message.
		 * 
		 * @param p_event The system message to handle.
		 */
		virtual void _handleMessage(SystemMessage& p_event) = 0;

	public:
		/**
		 * @brief Constructs the IMessageConsumerModule object.
		 * 
		 * @param p_queue The thread-safe queue for system messages.
		 */
		IMessageConsumerModule(spk::ThreadSafeQueue<SystemMessage> &p_queue);

		/**
		 * @brief Updates the IMessageConsumerModule object.
		 */
		void treatMessage();
	};
}
