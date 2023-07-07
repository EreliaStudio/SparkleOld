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
		struct WidgetDepthComparator {
			bool operator()(const AbstractWidget* lhs, const AbstractWidget* rhs) const
			{
				return lhs->depth() < rhs->depth();
			}
		};
		
		std::multiset<spk::AbstractWidget*, WidgetDepthComparator> _widgetSet;

		WidgetAtlas()
		{

		}

	public:
		void insert(AbstractWidget* p_widget)
		{
			_widgetSet.insert(p_widget);
		}

		void remove(AbstractWidget* p_widget)
		{
			auto it = _widgetSet.find(p_widget);
			
			if (it != _widgetSet.end()) {
				_widgetSet.erase(it);
			}
		}

		void update(AbstractWidget* p_widget)
		{
			auto it = _widgetSet.find(p_widget);
			
			if (it != _widgetSet.end()) {
				auto nodeHandler = _widgetSet.extract(it); // Extraire le widget sans le détruire.
				_widgetSet.insert(std::move(nodeHandler)); // Réinsérer le widget.
			}
		}

		const std::multiset<AbstractWidget*, WidgetDepthComparator>& getWidgetSet() const
		{
        	return _widgetSet;
    	}
	};
}