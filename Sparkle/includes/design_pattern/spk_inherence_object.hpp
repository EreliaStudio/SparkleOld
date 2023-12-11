#pragma once 

#include <memory>
#include <vector>
#include <functional>
#include <iostream>

namespace spk
{
	/**
	 * @brief InherenceObject class template.
	 *
	 * This class template represents an object that can be part of an inheritance hierarchy.
	 * It allows setting a parent object, adding child objects, and defining birth and death callbacks.
	 *
	 * @tparam TType The type of the object.
	 */
	template <typename TType>
	class InherenceObject
	{
	public:
		using Parent = TType*;						   /**< The type of the parent object. */
		using Child = TType*;							/**< The type of the child objects. */
		using ChildContainer = std::vector<Child>;		  /**< The type of child container used by InherenceObject to store them*/
		using Callback = std::function<void(Parent)>;	 /**< The type of the callback function. */

	private:
		Parent _parent;								 /**< The parent object. */
		ChildContainer _childrens;				   /**< The vector of child objects. */
		Callback _birthCallback;						 /**< The birth callback function. */
		Callback _deathCallback;						 /**< The death callback function. */

		void _addChild(Child child)
		{
			_childrens.push_back(child);
			if (child->_birthCallback != nullptr)
			{
				child->_birthCallback((TType*)(this));
			}
		}

		void _removeChild(Child child)
		{
			if (_deathCallback != nullptr)
				_deathCallback(child);
			for (auto it = _childrens.begin(); it != _childrens.end(); ++it)
			{
				if (*it == child)
				{
					_childrens.erase(it);
					break;
				}
			}
		}

	public:
		/**
		 * @brief Default constructor for InherenceObject.
		 *
		 * This constructor initializes the InherenceObject.
		 */
		InherenceObject() :
			_parent(nullptr),
			_childrens(),
			_birthCallback(nullptr),
			_deathCallback(nullptr)
		{

		}

		/**
		 * @brief Destructor for InherenceObject.
		 *
		 * This destructor removes the object from its parent's children list and sets the parent of all its children to nullptr.
		 */
		~InherenceObject()
		{
			if (_parent != nullptr)
				_parent->_removeChild(static_cast<Child>(this));
			for (auto child : _childrens)
				child->_parent = nullptr;
		}

		/**
		 * @brief Set the parent object.
		 *
		 * This function sets the parent object of the current object.
		 * It removes the object from its current parent's children list and adds it to the new parent's children list.
		 *
		 * @param parent The parent object to set.
		 */
		void setParent(Parent parent)
		{
			if (_parent != nullptr)
				_parent->_removeChild(static_cast<Child>(this));

			_parent = parent;

			if (_parent != nullptr)
				_parent->_addChild(static_cast<Child>(this));
		}

		/**
		 * @brief Add a child object.
		 *
		 * This function adds a child object to the current object.
		 * It sets the parent of the child object to the current object.
		 *
		 * @param child The child object to add.
		 */
		void addChild(Child child)
		{
			child->setParent(static_cast<Parent>(this));
		}

		/**
		 * @brief Set the birth callback.
		 *
		 * This function sets the birth callback function.
		 * The birth callback is called when a child object is added to the current object.
		 *
		 * @param callback The birth callback function to set.
		 */
		void setBirthCallback(std::function<void(Parent)> callback)
		{
			_birthCallback = callback;
		}

		/**
		 * @brief Set the death callback.
		 *
		 * This function sets the death callback function.
		 * The death callback is called when a child object is removed from the current object.
		 *
		 * @param callback The death callback function to set.
		 */
		void setDeathCallback(std::function<void(Child)> callback)
		{
			_deathCallback = callback;
		}

		/**
		 * @brief Get the parent object.
		 *
		 * This function returns a constant reference to the parent object of the current object.
		 *
		 * @return A constant reference to the parent object.
		 */
		constexpr const Parent& parent() const
		{
			return _parent;
		}

		/**
		 * @brief Get the vector of child objects.
		 *
		 * This function returns a reference to the vector of child objects of the current object.
		 *
		 * @return A reference to the vector of child objects.
		 */
		constexpr ChildContainer& childrens()
		{
			return _childrens;
		}

		/**
		 * @brief Get the vector of child objects (const version).
		 *
		 * This function returns a constant reference to the vector of child objects of the current object.
		 *
		 * @return A constant reference to the vector of child objects.
		 */
		constexpr const ChildContainer& childrens() const
		{
			return _childrens;
		}

		/**
		 * @brief Deleted copy constructor.
		 *
		 * This copy constructor is deleted to prevent copying an InherenceObject.
		 */
		InherenceObject(const InherenceObject&) = delete;

		/**
		 * @brief Deleted copy assignment operator.
		 *
		 * This copy assignment operator is deleted to prevent assigning an InherenceObject.
		 */
		InherenceObject& operator=(const InherenceObject&) = delete;
	};
}