#pragma once

#include "math/spk_vector2.hpp"
#include "application/modules/spk_message_consumer_module.hpp"
#include "graphics/spk_window.hpp"

namespace spk
{
	/**
	 * @class GraphicalAPIModule
	 * @brief Class for managing the application window.
	 * 
	 * This class manages the application window, including handling system messages related to the window,
	 * rendering the window, and clearing the window.
	 */
	class GraphicalAPIModule : public IMessageConsumerModule
	{
	private:
		spk::Window _window;

		/**
		 * @brief Handles a system message.
		 * 
		 * @param p_event The system message to handle.
		 */
		void _handleMessage(SystemMessage& p_event);

	public:
		/**
		 * @brief Constructs the GraphicalAPIModule object.
		 * 
		 * @param p_queue The thread-safe queue for window system messages.
		 * @param p_title The title of the window.
		 * @param p_size The size of the window.
		 * @param p_apiModule Pointer to the API Module. (Mandatory only on windows)
		 */
		GraphicalAPIModule(spk::ThreadSafeQueue<SystemMessage> &p_queue, const std::wstring& p_title, const spk::Vector2Int& p_size, void *p_apiModule = nullptr);

		/**
		 * @brief Destructs the GraphicalAPIModule object.
		 */
		~GraphicalAPIModule();

		/**
		 * @brief Renders the window.
		 */
		void render();

		/**
		 * @brief Clears the window.
		 */
		void clear();

		/**
		 * @brief Resizes the window to a new size.
		 * 
		 * @param p_newSize The new size for the window as a Vector2UInt.
		 */
		void resize(const spk::Vector2UInt& p_newSize);

		/**
		 * @brief Provides non-const access to the Window instance.
		 * 
		 * @return Reference to the Window instance.
		 */
		spk::Window& window() { return _window; }

		/**
		 * @brief Provides const access to the Window instance.
		 * 
		 * @return Const reference to the Window instance.
		 */
		const spk::Window& window() const { return _window; }
	};
}
