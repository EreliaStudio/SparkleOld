#pragma once

#include <set>
#include <memory>
#include "design_pattern/spk_singleton.hpp"
#include "widget/spk_abstract_widget.hpp"

namespace spk
{
	class WidgetAtlas : public spk::Singleton<WidgetAtlas>
	{
		friend class spk::Singleton<WidgetAtlas>;

	private:
		using StoringContainer = std::vector<AbstractWidget*>;
		StoringContainer _widgets;

		WidgetAtlas()
		{

		}

	public:
		void insert(AbstractWidget* p_widget)
		{
			_widgets.push_back(p_widget);
			sort(p_widget);
		}

		void remove(AbstractWidget* p_widget)
		{
			auto it = std::find(_widgets.begin(), _widgets.end(), p_widget);
			
			if (it != _widgets.end()) {
				_widgets.erase(it);
			}
		}

		void sort(AbstractWidget* p_widget)
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

				
		void printInfo()
		{
			for (const auto& widget : widgets()) {
				std::wcout << L"Widget: " << widget->name() << L", depth " << widget->depth() << L", Active: " << (widget->_isOperationnal ? L"True" : L"False") << std::endl;
			}
		}

		const StoringContainer& widgets() const
		{
        	return _widgets;
    	}
	};
}