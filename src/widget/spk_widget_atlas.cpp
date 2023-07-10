#include "widget/spk_widget_atlas.hpp"

namespace spk
{
	WidgetAtlas::WidgetAtlas()
		{

		}

		void WidgetAtlas::insert(AbstractWidget* p_widget)
		{
			_widgets.push_back(p_widget);
			sort(p_widget);
		}

		void WidgetAtlas::remove(AbstractWidget* p_widget)
		{
			auto it = std::find(_widgets.begin(), _widgets.end(), p_widget);
			
			if (it != _widgets.end()) {
				_widgets.erase(it);
			}
		}

		void WidgetAtlas::sort(AbstractWidget* p_widget)
		{
			auto it = std::find(_widgets.begin(), _widgets.end(), p_widget);
			
			if (it != _widgets.end()) {
				_widgets.erase(it);
				
				auto const insertion_point =
					std::upper_bound(_widgets.begin(), _widgets.end(), p_widget, [](const AbstractWidget* lhs, const AbstractWidget* rhs)
					{
						if (lhs->depth() == rhs->depth()) {
							return lhs < rhs;
						}
						return lhs->depth() < rhs->depth();
					});

				_widgets.insert(insertion_point, p_widget);
			}
		}

		const WidgetAtlas::StoringContainer& WidgetAtlas::widgets() const
		{
        	return _widgets;
    	}
}