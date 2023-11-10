#pragma once

#include "design_pattern/spk_contract_provider.hpp"

#include "iostream/spk_iostream.hpp"

namespace spk
{
	/**
	 * @brief A class template for managing a value with default and custom states.
	 *
	 * This class template represents a value that can have a default state and a custom state.
	 * The default state is managed by the inner class Default, and the custom state is managed
	 * by the template itself. The value can be subscribed to and notifies its subscribers when
	 * it is edited through callbacks.
	 *
	 * @tparam TType The type of the value.
	 */
	template <typename TType>
	class Value : public ContractProvider
	{
		friend class Default;

	public:
		/**
		 * @brief The inner class for managing the default state of the value.
		 *
		 * This inner class represents the default state of the value. It holds the actual value,
		 * keeps track of subscribers, and triggers edition callbacks when the value is changed.
		 */
		class Default
		{
			friend class Value;

		private:
			TType _value;							 /**< The actual value in the default state. */
			std::vector<Value<TType>*> _subscribers;   /**< The list of subscribers to the value. */

			/**
			 * @brief Subscribe to the value.
			 *
			 * This function adds a subscriber to the list of subscribers.
			 *
			 * @param p_subscriber The subscriber to add.
			 */
			void _subscribe(Value<TType>* p_subscriber)
			{
				_subscribers.push_back(p_subscriber);
			}

			/**
			 * @brief Unsubscribe from the value.
			 *
			 * This function removes a subscriber from the list of subscribers.
			 *
			 * @param p_subscriber The subscriber to remove.
			 */
			void _unsubscribe(Value<TType>* p_subscriber)
			{
				_subscribers.erase(std::remove(_subscribers.begin(), _subscribers.end(), p_subscriber), _subscribers.end());
			}

			/**
			 * @brief Trigger edition callbacks for subscribers.
			 *
			 * This function triggers the edition callbacks for all the subscribers
			 * whose state is Default.
			 */
			void _triggerSubscriberEditionCallbacks() const
			{
				for (size_t i = 0; i < _subscribers.size(); i++)
				{
					if (_subscribers[i]->_state == State::Default)
						_subscribers[i]->_triggerEditionCallback();
				}
			}

		public:
			/**
			 * @brief Construct a new Default object with a given initial value.
			 *
			 * @tparam Args The types of the arguments.
			 * @param p_args The arguments to initialize the value.
			 */
			template <typename... Args>
			Default(Args&&... p_args) :
				_value(std::forward<Args>(p_args)...),
				_subscribers()
			{

			}

			/**
			 * @brief Copy constructor for the Default object.
			 *
			 * @param p_other The Default object to copy.
			 */
			Default(const Default& p_other) :
				_value(p_other._value),
				_subscribers()
			{

			}

			/**
			 * @brief Destructor for the Default object.
			 *
			 * @note Throws a std::runtime_error if the Default value is still used by at least one Value.
			 */
			~Default() noexcept(false)
			{
				if (_subscribers.empty() == false)
					throw std::runtime_error("Destroying a Default value still used by at least one Value");
			}

			/**
			 * @brief Assignment operator for setting the value in the Default state.
			 *
			 * This operator sets the value in the Default state and triggers the edition
			 * callbacks for the subscribers.
			 *
			 * @param p_newValue The new value to set.
			 * @return A reference to the updated Default object.
			 */
			Default& operator=(const TType& p_newValue)
			{
				_value = p_newValue;
				_triggerSubscriberEditionCallbacks();
				return (*this);
			}

			/**
			 * @brief Copy assignment operator for the Default object.
			 *
			 * This operator copies the value from another Default object and triggers the edition
			 * callbacks for the subscribers.
			 *
			 * @param p_other The Default object to copy.
			 * @return A reference to the updated Default object.
			 */
			Default& operator=(const Default& p_other)
			{
				_value = p_other._value;
				_triggerSubscriberEditionCallbacks();
				return (*this);
			}

			/**
			 * @brief Conversion operator for accessing the value in the Default state.
			 *
			 * This conversion operator allows accessing the value in the Default state as
			 * a constant reference.
			 *
			 * @return A constant reference to the value in the Default state.
			 */
			operator const TType&() const
			{
				return (_value);
			}
		};

	private:
		enum class State
		{
			Default,
			Custom
		};

		CallbackContainer _onEditionCallbacks;  /**< The callbacks to trigger when the value is edited. */
		State _state;						    /**< The state of the value (Default or Custom). */
		std::shared_ptr<const Default> _default;/**< A pointer to the Default object. */
		TType _value;						    /**< The value in the custom state. */

		/**
		 * @brief Trigger edition callbacks for subscribers.
		 *
		 * This function triggers the edition callbacks for all the subscribers
		 * of the value.
		 */
		void _triggerEditionCallback()
		{
			for (size_t i = 0; i < _onEditionCallbacks.size(); i++)
			{
				_onEditionCallbacks[i]();
			}
		}

	public:
		/**
		 * @brief Construct a new Value object with a given default value.
		 *
		 * @param p_defaultValue The default value for the Value object.
		 */
		Value(std::shared_ptr<const Default> p_defaultValue) :
			_default(nullptr),
			_value(),
			_state(State::Default)
		{
			setDefaultValue(p_defaultValue);
		}

		/**
		 * @brief Construct a new Value object with a given default value and custom arguments.
		 *
		 * @tparam Args The types of the arguments.
		 * @param p_defaultValue The default value for the Value object.
		 * @param p_args The arguments to initialize the value in the custom state.
		 */
		template <typename... Args>
		Value(std::shared_ptr<const Default> p_defaultValue, Args&&... p_args) :
			_default(nullptr),
			_value(std::forward<Args>(p_args)...),
			_state(State::Custom)
		{
			setDefaultValue(p_defaultValue);
		}

		/**
		 * @brief Copy constructor for the Value object.
		 *
		 * @param p_other The Value object to copy.
		 */
		Value(const Value& p_other) :
			_default(nullptr),
			_value(p_other._value),
			_state(p_other._state)
		{
			setDefaultValue(p_other._default);
		}

		/**
		 * @brief Destructor for the Value object.
		 */
		~Value()
		{
			setDefaultValue(nullptr);
		}

		/**
		 * @brief Subscribe to the value for edition callbacks.
		 *
		 * This function subscribes a callback to the value for edition callbacks.
		 *
		 * @param p_callback The callback function to subscribe.
		 * @return A Contract object representing the subscription.
		 */
		Contract subscribe(const Callback& p_callback)
		{
			return (std::move(ContractProvider::subscribe(_onEditionCallbacks, p_callback)));
		}

		/**
		 * @brief Reset the value to the default state.
		 *
		 * This function resets the value to the default state and triggers the edition callbacks.
		 */
		void reset()
		{
			_state = State::Default;
			_triggerEditionCallback();
		}

		/**
		 * @brief Set the default value for the Value object.
		 *
		 * This function sets the default value for the Value object and subscribes/unsubscribes
		 * from the Default object as necessary.
		 *
		 * @param p_defaultValue The new default value.
		 */
		void setDefaultValue(std::shared_ptr<const Default> p_defaultValue)
		{
			if (_default != nullptr)
				const_cast<Default*>(_default.get())->_unsubscribe(this);
			_default = p_defaultValue;
			if (_default != nullptr)
				const_cast<Default*>(_default.get())->_subscribe(this);
		}

		/**
		 * @brief Copy assignment operator for the Value object.
		 *
		 * This operator copies the default value and the custom value from another Value object
		 * and triggers the edition callbacks.
		 *
		 * @param p_other The Value object to copy.
		 * @return A reference to the updated Value object.
		 */
		Value& operator=(const Value& p_other)
		{
			setDefaultValue(p_other._default);
			_value = p_other._value;
			_state = p_other._state;
			_triggerEditionCallback();
			return (*this);
		}

		/**
		 * @brief Assignment operator for setting the value in the custom state.
		 *
		 * This operator sets the value in the custom state and triggers the edition
		 * callbacks.
		 *
		 * @param p_newValue The new value to set.
		 * @return A reference to the updated Value object.
		 */
		Value& operator=(const TType& p_newValue)
		{
			_value = p_newValue;
			_state = State::Custom;
			_triggerEditionCallback();
			return (*this);
		}

		/**
		 * @brief Conversion operator for accessing the value.
		 *
		 * This conversion operator allows accessing the value as a constant reference.
		 * If the value is in the custom state, it returns the custom value; otherwise, it
		 * returns the default value.
		 *
		 * @return A constant reference to the value.
		 */
		operator const TType&() const
		{
			if (_state == State::Custom)
				return (_value);
			else
				return (*_default);
		}

		/**
		 * @brief Method for accessing the value.
		 *
		 * This method allows accessing the value as a constant reference.
		 * If the value is in the custom state, it returns the custom value; otherwise, it
		 * returns the default value.
		 *
		 * @return A constant reference to the value.
		 */
		const TType& value() const
		{
			if (_state == State::Custom)
				return (_value);
			else
				return (*_default);
		}
	};

	/**
	 * @brief A wrapper class for managing a value with automatic update notifications.
	 * 
	 * @tparam T The type of the value being wrapped.
	 */
	template <typename T>
	class ValueWrapper
	{
	public:
		/**
		 * @brief Alias for the default value type of the wrapped value.
		 */
		using Default = typename Value<T>::Default;
		
	private:
		/**
		 * @brief Flag to indicate whether the value needs an update.
		 */
		bool _needUpdate;

		/**
		 * @brief The actual value being wrapped.
		 */
		Value<T> _value;

		/**
		 * @brief Contract for subscription updates.
		 */
		Value<T>::Contract _contract;

		/**
		 * @brief Mutex for thread-safe access.
		 */
		std::recursive_mutex _mutex;

	public:
		/**
		 * @brief Constructs a new Value Wrapper object.
		 * 
		 * @param p_defaultValue The default value to initialize the wrapper with.
		 */
		ValueWrapper(const std::shared_ptr<const Default>& p_defaultValue) :
			_needUpdate(true),
			_value(p_defaultValue),
			_contract(_value.subscribe([&](){
				std::lock_guard<std::recursive_mutex> lockGuard(_mutex);
				_needUpdate = true;
			}))
		{
		}

		/**
		 * @brief Converts the wrapped value to its underlying type.
		 * 
		 * @return T The current value.
		 */
		operator T() const { return (_value.value()); }

		/**
		 * @brief Provides access to the wrapped value.
		 * 
		 * @return T& Reference to the current value.
		 */
		T& operator->() { return (_value.value()); }

		/**
		 * @brief Provides const access to the wrapped value.
		 * 
		 * @return const T& Const reference to the current value.
		 */
		const T& operator->() const { return (_value.value()); }
		
		/**
		 * @brief Assigns a new value to the wrapper.
		 * 
		 * @param p_rhs The right-hand side value to assign.
		 * @return ValueWrapper<T>& Reference to the updated object.
		 */
		ValueWrapper<T>& operator= (const T& p_rhs)
		{
			std::lock_guard<std::recursive_mutex> lockGuard(_mutex);
			_value = p_rhs;
			_needUpdate = true;
			return *this;
		}

		/**
		 * @brief Checks if the value needs an update.
		 * 
		 * @return bool True if the value needs an update.
		 */
		bool needUpdate() const { return (_needUpdate); }

		/**
		 * @brief Resets the update flag to false.
		 */
		void resetUpdateFlag() { 
			std::lock_guard<std::recursive_mutex> lockGuard(_mutex);
			_needUpdate = false; 
		}

		/**
		 * @brief Provides access to the wrapped Value object.
		 * 
		 * @return Value<T>& Reference to the wrapped value.
		 */
		Value<T>& operator()() { return (_value); }
		
		/**
		 * @brief Provides access to the wrapped value.
		 * 
		 * @return Value<T>& Reference to the wrapped value.
		 */
		Value<T>& value() { return (_value); }

		/**
		 * @brief Provides const access to the wrapped value.
		 * 
		 * @return const Value<T>& Const reference to the wrapped value.
		 */
		const Value<T>& value() const { return (_value); }
		
		/**
		 * @brief Gets the current value.
		 * 
		 * @return const T& Const reference to the current value.
		 */
		const T& get() const { return (_value.value()); }
	};

}
