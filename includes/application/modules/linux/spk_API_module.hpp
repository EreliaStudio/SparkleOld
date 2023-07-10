#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "spk_system_define.hpp"

namespace spk
{
	/**
	 * @class APIModule
	 * @brief Class for managing the API interactions.
	 * 
	 * This class handles the API interactions and updates for the window, mouse and keyboard.
	 * It uses thread-safe queues to ensure safe multi-threaded operations.
	 */
	class APIModule
	{
	private:
		spk::ThreadSafeQueue<SystemMessage> _windowQueue; ///< Thread-safe queue for window system messages.
		spk::ThreadSafeQueue<SystemMessage> _mouseQueue; ///< Thread-safe queue for mouse system messages.
		spk::ThreadSafeQueue<SystemMessage> _keyboardQueue; ///< Thread-safe queue for keyboard system messages.

	public:
		/**
		 * @brief Constructs the APIModule object.
		 */
		APIModule();
		
		/**
		 * @brief Destructs the APIModule object.
		 */
		~APIModule();

		/**
		 * @brief Updates the API module.
		 * 
		 * This method should be implemented in the .cpp file.
		 */
		void pullMessage();

		/**
		 * @brief Returns the thread-safe queue for window system messages.
		 * 
		 * @return Thread-safe queue for window system messages.
		 */
		spk::ThreadSafeQueue<SystemMessage> &windowQueue() { return _windowQueue; }

		/**
		 * @brief Returns the thread-safe queue for mouse system messages.
		 * 
		 * @return Thread-safe queue for mouse system messages.
		 */
		spk::ThreadSafeQueue<SystemMessage> &mouseQueue() { return _mouseQueue; }

		/**
		 * @brief Returns the thread-safe queue for keyboard system messages.
		 * 
		 * @return Thread-safe queue for keyboard system messages.
		 */
		spk::ThreadSafeQueue<SystemMessage> &keyboardQueue() { return _keyboardQueue; }
	};
}
