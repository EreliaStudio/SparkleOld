#pragma once

#include "design_pattern/spk_contract_provider.hpp"

namespace spk
{
	template <typename TType>
	class ObservableValue : public ContractProvider
	{
	private:
		TType _value;
		CallbackContainer _callbacks;

		void _notify()
		{
			for (auto& lambda : _callbacks)
				lambda();
		}

	public:
		template<typename ... Args>
		ObservableValue(Args&& ... p_args)
		{
			_value = TType(p_args...);
		}

		ObservableValue(const TType& p_value)
		{
			_value = p_value;
		}

		operator const TType&() const
		{
			return (_value);
		}

		const TType& value() const
        {
            return (_value);
        }

		std::shared_ptr<ContractProvider::Contract> subscribe(std::function<void()> p_callback)
		{
			return (ContractProvider::subscribe(_callbacks, p_callback));
		}

		template<typename TOtherType>
		bool operator==(const ObservableValue<TOtherType>& p_other) const
		{
			return _value == p_other._value;
		}

		template<typename TOtherType>
		bool operator!=(const ObservableValue<TOtherType>& p_other) const
		{
			return _value != p_other._value;
		}

		template<typename TOtherType>
		bool operator<(const ObservableValue<TOtherType>& p_other) const
		{
			return _value < p_other._value;
		}

		template<typename TOtherType>
		bool operator>(const ObservableValue<TOtherType>& p_other) const
		{
			return _value > p_other._value;
		}

		template<typename TOtherType>
		bool operator<=(const ObservableValue<TOtherType>& p_other) const
		{
			return _value <= p_other._value;
		}

		template<typename TOtherType>
		bool operator>=(const ObservableValue<TOtherType>& p_other) const
		{
			return _value >= p_other._value;
		}

		template<typename TOtherType>
		ObservableValue& operator = (const TOtherType& p_value)
        {
            if (_value != p_value)
            {
                _value = p_value;
                _notify();
            }
            return *this;
        }

		template<typename TOtherType>
		ObservableValue operator+(const TOtherType& p_other) const
		{
			return ObservableValue(_value + p_other);
		}

		template<typename TOtherType>
		ObservableValue& operator+=(const TOtherType& p_other)
		{
			_value += p_other;
			_notify();
			return *this;
		}

		template<typename TOtherType>
		ObservableValue operator-(const TOtherType& p_other) const
		{
			return ObservableValue(_value - p_other);
		}

		template<typename TOtherType>
		ObservableValue& operator-=(const TOtherType& p_other)
		{
			_value -= p_other;
			_notify();
			return *this;
		}

		template<typename TOtherType>
		ObservableValue operator*(const TOtherType& p_other) const
		{
			return ObservableValue(_value * p_other);
		}

		template<typename TOtherType>
		ObservableValue& operator*=(const TOtherType& p_other)
		{
			_value *= p_other;
			_notify();
			return *this;
		}

		template<typename TOtherType>
		ObservableValue operator/(const TOtherType& p_other) const
		{
			return ObservableValue(_value / p_other);
		}

		template<typename TOtherType>
		ObservableValue& operator/=(const TOtherType& p_other)
		{
			_value /= p_other;
			_notify();
			return *this;
		}
	};
}