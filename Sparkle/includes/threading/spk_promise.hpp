#pragma once

#include <future>
#include <optional>
#include "design_pattern/spk_callback_container.hpp"

namespace spk
{
	/**
	 * @class Promise
	 * @brief Class for providing a promise with a value.
	 * 
	 * This class provides a promise that can be fulfilled with a value. 
	 * It allows subscribing to the promise to receive the value when it becomes available.
	 * 
	 * @tparam TType The type of the value.
	 */
	template<typename TType>
	class Promise
	{
	public:
		/**
		 * @brief Alias for CallbackContainer::Callback.
		 *
		 * Callback is used to define the function signature for callback functions that can be subscribed to
		 * the Promise class. This allows clients of the Promise class to register functions to be called
		 * when the promise is fulfilled and the value becomes available.
		 */
		using Callback = CallbackContainer::Callback;

		/**
		 * @brief Alias for CallbackContainer::Contract.
		 *
		 * Contract represents a handle or token that manages the lifecycle of a callback subscription
		 * within the Promise class. It provides a means for clients to manage their subscriptions,
		 * such as removing or modifying a callback after it has been added.
		 */
		using Contract = CallbackContainer::Contract;

	private:
		CallbackContainer _callbackContainer;
		std::promise<TType> _promise;
		std::optional<TType> _value;

	public:
		/**
		 * @brief Construct a new Promise object.
		 * 
		 * This is the default constructor for the Promise class.
		 */
		Promise() :
			_callbackContainer(),
			_promise(),
			_value()
		{
		}

		/**
		 * @brief Copy constructor is deleted.
		 * 
		 * Copying a Promise is not allowed to avoid potential issues with the shared state.
		 */
		Promise(const Promise& p_other) = delete;

		/**
		 * @brief Assignment operator is deleted.
		 * 
		 * Assigning a Promise is not allowed to avoid potential issues with the shared state.
		 */
		Promise& operator=(const Promise& p_other) = delete;

		/**
		 * @brief Subscribes a callback to be notified when the promise is fulfilled.
		 *
		 * This method allows clients to register a callback function that will be invoked when the promise
		 * is fulfilled and the value becomes available. This is useful for implementing reactive behaviors
		 * where certain actions depend on the availability of the promised value.
		 *
		 * @param p_callback The callback function to be executed when the promise is fulfilled.
		 * @return Contract A contract object representing the registration of the callback. This contract
		 *         can be used for managing the callback, such as modifying or removing it at a later stage.
		 */
		Contract subscribe(const Callback& p_callback)
		{
			return (_callbackContainer.subscribe(p_callback));
		}

		/**
		 * @brief Check if the promise has been fulfilled.
		 * 
		 * This function checks if the promise has been fulfilled and the value is available.
		 * 
		 * @return True if the promise has been fulfilled, false otherwise.
		 */
		constexpr bool realised() const
		{
			return (_value.has_value());
		}

		/**
		 * @brief Set the value of the promise.
		 * 
		 * This function sets the value of the promise. Once the value is set, all the subscribed 
		 * callbacks will be called.
		 * 
		 * @param p_value The value to set.
		 */
		void setValue(const TType& p_value)
		{
			if (realised() == true)
			{
				throw std::runtime_error("Promise already realised");
			}
			_promise.set_value(p_value);
			_callbackContainer.notify();
		}

		/**
		 * @brief Get the value of the promise.
		 * 
		 * This function gets the value of the promise. If the value is not available yet, 
		 * it will wait for the future to be ready and then return the value.
		 * 
		 * @return The value of the promise.
		 */
		constexpr const TType& value()
		{
			if (_value.has_value() == false)
			{
				_value = _promise.get_future().get();
			}
			return (_value.value());
		}
	};
}