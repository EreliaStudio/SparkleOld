#pragma once

#include <set>
#include <memory>
#include "design_pattern/spk_singleton.hpp"
#include "widget/spk_abstract_widget.hpp"

namespace spk
{
    /**
     * @class WidgetAtlas
     * @brief A singleton atlas of widgets.
     *
     * WidgetAtlas is a class that acts as a collection of AbstractWidget pointers. It provides methods 
     * for inserting, removing, and sorting widgets, as well as accessing the collection itself. Being a
     * Singleton, it ensures there is only one instance of this collection in the program.
	 * 
	 * This class is called automaticaly by the widget when they are created or when they have their depth edited.
	 * This singleton is created inside the WidgetModule.
     */
	class WidgetAtlas : public spk::Singleton<WidgetAtlas>
	{
		friend class spk::Singleton<WidgetAtlas>;

	private:
		/**
		 * @brief A type definition for the container storing widgets.
		 */
		using StoringContainer = std::vector<AbstractWidget*>;
		/**
		 * @brief Collection of AbstractWidget pointers.
		 */
		StoringContainer _widgets;

		/**
		 * @brief Private default constructor to enforce Singleton pattern.
		 */
		WidgetAtlas();

	public:
		/**
		 * @brief Inserts a new widget into the collection.
		 *
		 * @param p_widget A pointer to the widget to be added.
		 */
		void insert(AbstractWidget* p_widget);

		/**
		 * @brief Removes a widget from the collection.
		 *
		 * @param p_widget A pointer to the widget to be removed.
		 */
		void remove(AbstractWidget* p_widget);

		/**
		 * @brief Sorts the widgets in the collection based on some criterion.
		 *
		 * @param p_widget A pointer to the widget that provides the sorting criterion.
		 */
		void sort(AbstractWidget* p_widget);

		/**
		 * @brief Returns a constant reference to the collection of widgets.
		 *
		 * @return const reference to the StoringContainer.
		 */
		const StoringContainer& widgets() const;
	};
}
