#pragma once

#include "widget/spk_abstract_widget.hpp"
#include "application/modules/spk_abstract_module.hpp"

namespace spk
{
	class WidgetModule : public spk::AbstractModule
	{
	private:
		class CentralWidget : public AbstractWidget
		{
			friend class WidgetModule;
			
		private:
			virtual void _onRender();
			virtual bool _onUpdate();

			virtual void _onGeometryChange();
		public:
			CentralWidget(std::wstring p_name);
		};

		CentralWidget* _centralWidget;

	public:
		WidgetModule();

		~WidgetModule();
		
		void update();

		void render();

		AbstractWidget* centralWidget() const;
	};
}