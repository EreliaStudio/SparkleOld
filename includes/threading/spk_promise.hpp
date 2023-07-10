#pragma once

#include <future>
#include <optional>

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
	class Promise : public ContractProvider
	{
	private:
		ContractProvider::CallbackContainer _callbackContainer;
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
		 * @brief Subscribe to the promise and receive the value when available.
		 * 
		 * This function allows subscribing to the promise to receive the value when it becomes available.
		 * 
		 * @param p_callback The callback function to be called when the value is available.
		 * @return A contract representing the subscription.
		 */
		Contract subscribe(const Callback& p_callback)
		{
			return ContractProvider::subscribe(_callbackContainer, p_callback);
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
			for (auto& callback : _callbackContainer)
			{
				callback();
			}
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