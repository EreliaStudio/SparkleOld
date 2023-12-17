#pragma once

#include <vector>

#include "widget/spk_widget.hpp"

namespace spk
{
    /**
     * @class WidgetModule
     * @brief Manages widgets within the Sparkle application framework.
     *
     * This class is responsible for creating, managing, and orchestrating the lifecycle of widgets,
     * including rendering and updating operations. It maintains a central widget as the root of the widget hierarchy.
     */
    class WidgetModule
    {
    public:
        /**
         * @class CentralWidget
         * @brief Represents the central widget in the application, derived from the Widget class.
         *
         * This subclass of Widget acts as the main container for other widgets. It overrides
         * essential widget functionalities like rendering, updating, and handling geometry changes.
         */
        class CentralWidget : public Widget
        {
        private:
            void _onGeometryChange(); /**< Handles geometry changes of the central widget. */
            void _onRender(); /**< Renders the central widget. */
            bool _onUpdate(); /**< Updates the central widget and returns true if an update occurred. */

        public:
            CentralWidget(); /**< Constructor for CentralWidget. */
            ~CentralWidget(); /**< Destructor for CentralWidget. */
        };

    private:
        CentralWidget* _centralWidget; /**< Pointer to the central widget of the module. */

        /**
         * @brief Applies a resize operation to a given widget.
         */
        void _applyWidgetResizeOperation(Widget* p_widget);

    public:
        /**
         * @brief Constructor for WidgetModul.
         */
        WidgetModule();

        /**
         * @brief Creates the central widget.
         */
        void createCentralWidget();

        /**
         * @brief Accessor for the central widget.
         * @return Pointer to the central widget.
         */
        CentralWidget* centralWidget();

        /**
         * @brief Searches for a widget by name within the widget hierarchy.
         * @param p_name The name of the widget to search for.
         * @return Pointer to the found widget, or nullptr if not found.
         */
        spk::Widget* searchWidget(const std::wstring& p_name) const;

		/**
		 * @brief Applies a resize operation to all managed widgets.
		 */
        void applyResizeOperation();

		/**
		 * @brief Renders all managed widgets.
		 */
        void renderWidgets(); /**< . */

		/**
		 * @brief Updates all managed widgets.
		 */
        void updateWidgets();
    };
}
