#pragma once

namespace spk
{
	/**
	 * @class WidgetModule
	 * @brief Class for managing widgets within the application.
	 *
	 * This class provides methods to handle and manage the widgets within the application.
	 */
	class WidgetModule
	{
	private:

	public:
		/**
		 * @brief Constructs the WidgetModule object.
		 */
		WidgetModule();

		/**
		 * @brief Destructs the WidgetModule object.
		 */
		~WidgetModule();

		/**
		 * @brief Updates the widget state.
		 */
		void update();

		/**
		 * @brief Renders the widget.
		 */
		void render();
	};
}
