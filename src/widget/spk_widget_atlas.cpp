#include "widget/spk_widget_atlas.hpp"

namespace spk::Widget
{
	Atlas::Atlas()
	{

	}

	void Atlas::insert(Interface* p_widget)
	{
		_widgets.push_back(p_widget);
		sort(p_widget);
	}

	void Atlas::remove(Interface* p_widget)
	{
		auto it = std::find(_widgets.begin(), _widgets.end(), p_widget);
		
		if (it != _widgets.end()) {
			_widgets.erase(it);
		}
	}

	void Atlas::sort(Interface* p_widget)
	{
		auto it = std::find(_widgets.begin(), _widgets.end(), p_widget);
		
		if (it != _widgets.end()) {
			_widgets.erase(it);
			
			auto const insertion_point =
				std::upper_bound(_widgets.begin(), _widgets.end(), p_widget, [](const Interface* lhs, const Interface* rhs)
				{
					if (lhs->depth() == rhs->depth()) {
						return lhs < rhs;
					}
					return lhs->depth() < rhs->depth();
				});

			_widgets.insert(insertion_point, p_widget);
		}
	}

	const Atlas::StoringContainer& Atlas::widgets() const
	{
		return _widgets;
	}

	void Atlas::resize(const spk::Vector2& p_resizeRatio)
	{
		spk::cout << "Resize screen by : " << p_resizeRatio << std::endl;
		for (size_t i = 0; i < _widgets.size(); i++)
		{
			_widgets[i]->setGeometry(_widgets[i]->anchor() * p_resizeRatio, _widgets[i]->size() * p_resizeRatio);
		}
	}
}