#pragma once

#include <set>
#include <memory>
#include "design_pattern/spk_singleton.hpp"
#include "widget/spk_widget_interface.hpp"

#define CONCATENATE_DETAIL(x, y, z) x ## y ## z
#define CONCATENATE(x, y, z) CONCATENATE_DETAIL(x, y, z)

#define registerClass(RegName, ClassName)                                               \
    struct CONCATENATE(Atlas_, ClassName, CONCATENATE(_, __LINE__, _Registrar))         \
    {                                                                                  \
        CONCATENATE(Atlas_, ClassName, CONCATENATE(_, __LINE__, _Registrar))()          \
        {                                                                              \
            spk::Widget::Atlas::classInstanciatorLambda[RegName] =                     \
                [](const std::wstring &p_name,                                         \
                   const spk::JSON::Object &p_obj) -> spk::Widget::Interface*          \
                {                                                                      \
                    return new ClassName(p_name, p_obj);                                \
                };                                                                     \
        }                                                                              \
    };                                                                                 \
    static inline CONCATENATE(Atlas_, ClassName, CONCATENATE(_, __LINE__, _Registrar)) \
            CONCATENATE(Atlas_, ClassName, CONCATENATE(_, __LINE__, _Instance)) =      \
                CONCATENATE(Atlas_, ClassName, CONCATENATE(_, __LINE__, _Registrar))()


namespace spk::Widget
{
	/**
	 * @class Atlas
	 * @brief A singleton atlas of widgets.
	 *
	 * Atlas is a class that acts as a collection of Interface pointers. It provides methods 
	 * for inserting, removing, and sorting widgets, as well as accessing the collection itself. Being a
	 * Singleton, it ensures there is only one instance of this collection in the program.
	 * 
	 * This class is called automaticaly by the widget when they are created or when they have their depth edited.
	 * This singleton is created inside the WidgetModule.
	 */
	class Atlas : public spk::Singleton<Atlas>
	{
		friend class spk::Singleton<Atlas>;

	public:
		/**
		 * @brief Type alias for the lambda function used to create instances of widget classes.
		 */
		using Instanciator = std::function<Interface*(const std::wstring&, const spk::JSON::Object&)>;
		/**
		 * @brief Static map to associate widget class names with instantiation lambdas.
		 */
		static inline std::map<std::wstring, Instanciator> classInstanciatorLambda;

	private:

		/**
		 * @brief A type definition for the container storing widgets.
		 */
		using StoringContainer = std::vector<Interface*>;
		/**
		 * @brief Collection of Interface pointers.
		 */
		StoringContainer _widgets;

		/**
		 * @brief Private default constructor to enforce Singleton pattern.
		 */
		Atlas();

	public:
		/**
		 * @brief Inserts a new widget into the collection.
		 *
		 * @param p_widget A pointer to the widget to be added.
		 */
		void insert(Interface* p_widget);

		/**
		 * @brief Removes a widget from the collection.
		 *
		 * @param p_widget A pointer to the widget to be removed.
		 */
		void remove(Interface* p_widget);

		/**
		 * @brief Sorts the widgets in the collection based on some criterion.
		 *
		 * @param p_widget A pointer to the widget that provides the sorting criterion.
		 */
		void sort(Interface* p_widget);

		/**
		 * @brief Get the desired widget.
		 * @param p_widgetName The target widget's name.
		 * @return Return a pointer to the desired widget if it exist, nullptr otherwise.
		*/
		Interface* get(const std::wstring& p_widgetName);

		/**
		 * @brief Returns a constant reference to the collection of widgets.
		 *
		 * @return const reference to the StoringContainer.
		 */
		const StoringContainer& widgets() const;
		
		/**
		 * @brief Resize every widget using a ratio.
		 * @param p_resizeRatio The ratio between the old and new screen size
		*/
		void resize(const spk::Vector2& p_resizeRatio);
	};
}
