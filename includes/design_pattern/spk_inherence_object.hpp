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
<<<<<<< HEAD
		{
			_childrens.push_back(std::move(p_child));

    		std::shared_ptr<TType> result(_childrens.back().get(), [](TType*)->void {});
			if (_birthCallback != nullptr)
				_birthCallback(result);
			return (result);
		}
=======
        {
            _childrens.push_back(std::move(p_child));

            std::shared_ptr<TType> result(_childrens.back().get(), [](TType*)->void {});
            if (_birthCallback != nullptr)
                _birthCallback(result);
            return (result);
        }
>>>>>>> a653da6ae6e8a826d15a26319d45c35b06b34304

		Child _removeChild(ChildReference p_child)
		{
			for (auto& it = _childrens.begin(); it != _childrens.end(); ++it)
			{
				if (*it == p_child)
				{
					Child result = std::move(*it);
					_childrens.erase(it);
					return result;
				}
			}
			return nullptr;
		}

	public:
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

		void setBirthCallback(std::function<void(Child)> callback)
		{
			_birthCallback = callback;
		}

		void setDeathCallback(std::function<void(Child)> callback)
		{
			_deathCallback = callback;
		}

		void setOrphanageCallback(std::function<void(Child)> callback)
		{
			_orphanageCallback = callback;
		}

		constexpr const Parent& parent() const
		{
			return _parent;
		}

		constexpr ChildContainer& childrens()
		{
			return _childrens;
		}

		constexpr const ChildContainer& childrens() const
		{
			return _childrens;
		}

		InherenceObject(InherenceObject&&) = default;
		InherenceObject& operator=(InherenceObject&&) = default;

		InherenceObject(const InherenceObject&) = delete;
		InherenceObject& operator=(const InherenceObject&) = delete;
	};
}