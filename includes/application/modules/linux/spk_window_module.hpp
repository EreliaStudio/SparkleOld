#pragma once

#include "math/spk_vector2.hpp"
#include "application/modules/spk_message_consumer_module.hpp"

namespace spk
{
	/**
	 * @class WindowModule
	 * @brief Class for managing the application window.
	 * 
	 * This class manages the application window, including handling system messages related to the window,
	 * rendering the window, and clearing the window.
	 */
	class WindowModule : public IMessageConsumerModule
	{
	private:
		/**
		 * @brief Handles a system message.
		 * 
		 * @param p_event The system message to handle.
		 */
		void _handleMessage(SystemMessage& p_event);

	public:
		/**
		 * @brief Constructs the WindowModule object.
		 * 
		 * @param p_queue The thread-safe queue for window system messages.
		 * @param p_title The title of the window.
		 * @param p_size The size of the window.
		 */
		WindowModule(spk::ThreadSafeQueue<SystemMessage> &p_queue, const std::wstring& p_title, const spk::Vector2Int& p_size);

		/**
		 * @brief Destructs the WindowModule object.
		 */
		~WindowModule();

		/**
		 * @brief Renders the window.
		 */
		void render();

		/**
		 * @brief Clears the window.
		 */
		void clear();
	};
}
