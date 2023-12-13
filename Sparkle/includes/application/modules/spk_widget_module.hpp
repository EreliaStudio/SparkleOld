#pragma once

#include <vector>

#include "widget/spk_widget_atlas.hpp"

namespace spk
{
	namespace Widget
	{
		class Interface;
	}

	/**
	 * @class WidgetModule
	 * @brief Class for managing widgets within the application.
	 *
	 * This class provides methods to handle and manage the widgets within the application.
	 */
	class WidgetModule
	{
	private:
		Widget::Atlas::StoringContainer* _widgetSet;

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
