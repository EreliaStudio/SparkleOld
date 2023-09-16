#include "widget/spk_widget_atlas.hpp"

namespace spk::Widget
{
	Atlas::Atlas()
	{

	}

	void Atlas::insert(std::shared_ptr<Interface> p_widget)
	{
		_widgets.push_back(p_widget);
		sort(p_widget);
	}

	void Atlas::remove(std::shared_ptr<Interface> p_widget)
	{
		auto it = std::find(_widgets.begin(), _widgets.end(), p_widget);
		
		if (it != _widgets.end()) {
			_widgets.erase(it);
		}
	}

	void Atlas::sort(std::shared_ptr<Interface> p_widget)
	{
		auto it = std::find(_widgets.begin(), _widgets.end(), p_widget);
		
		if (it != _widgets.end()) {
			_widgets.erase(it);
			
			auto const insertion_point =
				std::upper_bound(_widgets.begin(), _widgets.end(), p_widget, [](const std::shared_ptr<Interface> lhs, const std::shared_ptr<Interface> rhs)
				{
					if (lhs->depth() == rhs->depth()) {
						return lhs < rhs;
					}
					return lhs->depth() < rhs->depth();
				});

			_widgets.insert(insertion_point, p_widget);
		}
	}
	
	std::shared_ptr<Interface> Atlas::get(const std::wstring& p_widgetName)
	{
		auto tmp = std::find_if(_widgets.begin(), _widgets.end(), [&p_widgetName](std::shared_ptr<Interface> widget) {
			return (widget->name() == p_widgetName);
		});

		if (tmp != _widgets.end())
		{
			return *tmp;
		}
		return nullptr;
	}

	const Atlas::StoringContainer& Atlas::widgets() const
	{
		return _widgets;
	}

	void Atlas::resize(const spk::Vector2& p_resizeRatio)
	{
		for (auto it = _widgets.rbegin(); it != _widgets.rend(); ++it)
		{
			(*it)->setGeometry((*it)->anchor() * p_resizeRatio, (*it)->size() * p_resizeRatio);
		}
	}
}