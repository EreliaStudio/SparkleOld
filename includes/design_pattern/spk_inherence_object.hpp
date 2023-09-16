#pragma once 

#include <memory>
#include <vector>
#include <functional>
#include <cassert>

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
	class InherenceObject : public std::enable_shared_from_this<TType>
	{
	public:
		using Parent = std::shared_ptr<TType>;		   /**< The type of the parent object. */
		using Child = std::unique_ptr<TType>;							/**< The type of the child objects. */
		using ChildReference = std::shared_ptr<TType>;				  /**< The type of the child objects. */
		using ChildContainer = std::vector<Child>;		  /**< The type of child container used by InherenceObject to store them*/
		using Callback = std::function<void(ChildReference)>;	 /**< The type of the callback function. */

	private:
		Parent _parent;								 /**< The parent object. */
		ChildContainer _childrens;				   /**< The vector of child objects. */
		Callback _birthCallback;						 /**< The birth callback function. */
		Callback _deathCallback;						 /**< The death callback function. */
		Callback _orphanageCallback;					 /**< The orphanage callback function. */

		ChildReference _addChild(Child p_child)
        {
            _childrens.push_back(std::move(p_child));

            std::shared_ptr<TType> result(_childrens.back().get(), [](TType*)->void {});
            if (_birthCallback != nullptr)
                _birthCallback(result);
            return (result);
        }

		Child _removeChild(ChildReference child)
		{
			for (auto& it = _childrens.begin(); it != _childrens.end(); ++it)
			{
				if (*it == child)
				{
					Child result = std::move(*it);
					_childrens.erase(it);
					return result;
				}
			}
			return nullptr;
		}

	public:
		/**
		 * @brief Default constructor for InherenceObject.
		 *
		 * This constructor initializes the InherenceObject.
		 */
		InherenceObject() :
			_parent(nullptr),
			_birthCallback(nullptr),
			_deathCallback(nullptr),
			_orphanageCallback(nullptr)
		{

		}

		~InherenceObject()
		{
			if (_deathCallback != nullptr)
				_deathCallback(this->shared_from_this());
		}

		/**
		 * @brief Add a child object.
		 *
		 * This function adds a child object to the current object.
		 * It sets the parent of the child object to the current object.
		 *
		 * @param child The child object to add.
		 */
		ChildReference addChild(Child child)
		{
			assert(child->_parent == nullptr && "Child already has a parent");

			auto result = _addChild(std::move(child));
			result->_parent = this->shared_from_this();
			return (result);
		}

		Child removeChild(ChildReference child)
		{
			Child result = _removeChild(child);
			assert(result != nullptr && "Child not found");
			result->_parent = nullptr;
			if (_orphanageCallback != nullptr)
				_orphanageCallback(result);
			return result;
		}

		/**
		 * @brief Set the birth callback.
		 *
		 * This function sets the birth callback function.
		 * The birth callback is called when a child object is added to the current object.
		 *
		 * @param callback The birth callback function to set.
		 */
		void setBirthCallback(std::function<void(Child)> callback)
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

		void setOrphanageCallback(std::function<void(Child)> callback)
		{
			_orphanageCallback = callback;
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

		InherenceObject(InherenceObject&&) = default;
		InherenceObject& operator=(InherenceObject&&) = default;

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