#pragma once

#include "widget/spk_abstract_widget.hpp"
#include "application/modules/spk_abstract_module.hpp"

namespace spk
{
    /**
     * @class WidgetModule
     * @brief Class for managing widgets within the application.
     *
     * This class provides methods to handle and manage the widgets within the application.
     */
	class WidgetModule : public spk::AbstractModule
	{
	private:
        /**
         * @class CentralWidget
         * @brief Inner class that defines the central widget.
         *
         * This class is used to define and manage the central widget of the application.
         */
		class CentralWidget : public AbstractWidget
		{
			friend class WidgetModule;
			
		private:
            /**
             * @brief Handles the rendering of the widget.
             */
			virtual void _onRender();

            /**
             * @brief Handles the update logic of the widget.
             *
             * @return A boolean value indicating whether the update was successful.
             */
			virtual bool _onUpdate();

            /**
             * @brief Handles the logic when the geometry of the widget changes.
             */
			virtual void _onGeometryChange();

		public:
            /**
             * @brief Constructs a CentralWidget object.
             *
             * @param p_name The name of the widget.
             */
			CentralWidget(const std::wstring& p_name);
		};

        /**
         * @brief Pointer to the central widget of the application.
         */
		CentralWidget* _centralWidget;

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

        /**
         * @brief Returns a pointer to the central widget.
         *
         * @return Pointer to the central widget.
         */
		AbstractWidget* centralWidget() const;
	};
}
