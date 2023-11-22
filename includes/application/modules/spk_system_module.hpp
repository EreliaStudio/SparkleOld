#pragma once

#include "application/modules/spk_message_consumer_module.hpp"

namespace spk
{
	class Application;
	
	/**
	 * @class SystemModule
	 * @brief Class for managing system-level interactions.
	 *
	 * This class manages system-level interactions, including handling system messages and managing the main application.
	 */
	class SystemModule : public IMessageConsumerModule
	{
	private:
		/**
		 * @brief Pointer to the application this module is managing.
		 */
		spk::Application* _applicationToManage;

		/**
		 * @brief Handles a system message.
		 * 
		 * @param p_event The system message to handle.
		 */
		void _handleMessage(SystemMessage& p_event);

	public:
		/**
		 * @brief Constructs the SystemModule object.
		 * 
		 * @param p_queue The thread-safe queue for system messages.
		 * @param p_applicationToManage Pointer to the application this module will manage.
		 */
		SystemModule(ThreadSafeQueue<SystemMessage> &p_queue, spk::Application* p_applicationToManage);
		
		/**
		 * @brief Destructs the SystemModule object.
		 */
		~SystemModule();
	};
}
