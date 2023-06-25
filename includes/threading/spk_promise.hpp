#pragma once

#include <future>
#include <optional>

namespace spk
{
	template<typename TType>
	class Promise : public ContractProvider
	{
	private:
		ContractProvider::CallbackContainer _callbackContainer;
		std::promise<TType> _promise;
		std::optional<TType> _value;

	public:
		Promise() :
			_callbackContainer(),
			_promise(),
			_value()
		{
		}

		Promise(const Promise& p_other) = delete;
		Promise& operator=(const Promise& p_other) = delete;

		~Promise()
		{
		}

		Contract subscribe(const Callback& p_callback)
		{
			return ContractProvider::subscribe(_callbackContainer, p_callback);
		}

		constexpr bool realised() const
		{
			return (_value.has_value());
		}

		/**
		 * @brief Set the promise value
		 *
		 * @throw std::runtime_error if the promise is already realised
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
		 * @brief Try to retrieve the value of the promise
		 *
		 * @return const TType&
		 * @warning This function will block until the promise is realised
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