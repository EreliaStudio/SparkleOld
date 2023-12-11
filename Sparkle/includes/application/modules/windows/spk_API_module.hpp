#pragma once

#include "spk_system_define.hpp"
#include "design_pattern/spk_singleton.hpp"
#include "threading/spk_thread_safe_queue.hpp"
#include "data_structure/spk_pool.hpp"

namespace spk
{
	/**
	 * @class APIModule
	 * @brief Class for managing the API interactions.
	 * 
	 * This class handles the API interactions and updates for system, window, mouse, and keyboard.
	 * It uses a pool of DataBuffer as a singleton instance and thread-safe queues to ensure safe multi-threaded operations.
	 */
	class APIModule
	{
	private:
		using MessagePool = spk::Pool<spk::DataBuffer>; ///< Type definition for pool of DataBuffer.

		MessagePool _messagePool;

		spk::ThreadSafeQueue<MessagePool::Object> _systemQueue; ///< Thread-safe queue for system messages.
		spk::ThreadSafeQueue<MessagePool::Object> _windowQueue; ///< Thread-safe queue for window system messages.
		spk::ThreadSafeQueue<MessagePool::Object> _mouseQueue; ///< Thread-safe queue for mouse system messages.
		spk::ThreadSafeQueue<MessagePool::Object> _keyboardQueue; ///< Thread-safe queue for keyboard system messages.

		MessagePool::Object _obtain(const UINT& p_uMsg);

		SystemMessage _obtainMessage(const UINT& p_messageID);

	public:
		/**
		 * @brief Handles a system message.
		 * 
		 * @param p_hwnd Handle to the window.
		 * @param p_uMsg The message.
		 * @param p_wParam Additional message-specific information.
		 * @param p_lParam Additional message-specific information.
		 * @return The result of the message processing and depends on the message sent.
		 */
		LRESULT handleMessage(const HWND& p_hwnd, const UINT& p_uMsg, const WPARAM& p_wParam, const LPARAM& p_lParam);

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
		 */
		void pullMessage();

		/**
		 * @brief Returns the thread-safe queue for system messages.
		 * 
		 * @return Thread-safe queue for system messages.
		 */
		spk::ThreadSafeQueue<SystemMessage> &systemQueue() { return _systemQueue; }

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
	/**
	 * @brief Processes messages sent to a window.
	 * 
	 * @param p_hwnd Handle to the window.
	 * @param p_uMsg The message.
	 * @param p_wParam Additional message-specific information.
	 * @param p_lParam Additional message-specific information.
	 * @return The result of the message processing and depends on the message sent.
	 */
	LRESULT CALLBACK WindowProc(HWND p_hwnd, UINT p_uMsg, WPARAM p_wParam, LPARAM p_lParam);
	
}
