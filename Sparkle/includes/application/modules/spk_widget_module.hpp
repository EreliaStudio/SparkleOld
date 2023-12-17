#pragma once

#include <vector>

#include "widget/spk_widget.hpp"

namespace spk
{
	class WidgetModule
	{
	public:
		class CentralWidget : public Widget
		{
		private:
			void _onGeometryChange();

			void _onRender();
			bool _onUpdate();

		public:
			CentralWidget();
			~CentralWidget();
		};

	private:
		CentralWidget* _centralWidget;

		void _applyWidgetResizeOperation(Widget* p_widget);

	public:
		WidgetModule();

		void createCentralWidget();

		CentralWidget* centralWidget();

		spk::Widget* searchWidget(const std::wstring& p_name) const;

		void applyResizeOperation();

		void renderWidgets();
		void updateWidgets();
	};
}
