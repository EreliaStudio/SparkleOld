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

		WidgetAtlas();

	public:
		void insert(AbstractWidget* p_widget);

		void remove(AbstractWidget* p_widget);

		void sort(AbstractWidget* p_widget);

		const StoringContainer& widgets() const;
	};
}