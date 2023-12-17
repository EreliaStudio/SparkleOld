#pragma once

#include "design_pattern/spk_callback_container.hpp"

namespace spk
{
    /**
     * @brief A template class for creating observable values.
     *
     * ObservableValue allows for the creation of a value that can be observed for changes.
     * When the value changes, all subscribed callbacks are automatically called.
     *
     * @tparam TType The type of the value to be observed.
     */
    template <typename TType>
    class ObservableValue
    {
	public:
		using Contract = CallbackContainer::Contract;

    private:
        TType _value; ///< The value being observed.
        CallbackContainer _callbacks; ///< Container for callback functions.

	public:
	    /**
         * @brief Constructs an ObservableValue with provided arguments.
         *
         * @tparam Args Types of the arguments.
         * @param p_args Arguments to construct the TType object.
         */
		template<typename ... Args>
		ObservableValue(Args&& ... p_args)
		{
			_value = TType(p_args...);
		}

		/**
         * @brief Constructs an ObservableValue with an initial value.
         *
         * @param p_value Initial value of type TType.
         */
		ObservableValue(const TType& p_value)
		{
			_value = p_value;
		}

		/**
		 * @brief Conversion operator to const TType&.
		 *
		 * Allows the ObservableValue object to be used in contexts where a TType
		 * reference is expected. It enables implicit conversion to the underlying type.
		 * 
		 * @return const TType& A constant reference to the encapsulated value.
		 */
		operator const TType&() const
		{
			return (_value);
		}

		/**
		 * @brief Gets the current value.
		 *
		 * Provides direct access to the current value of the observable object.
		 * 
		 * @return const TType& A constant reference to the current value.
		 */
		const TType& value() const
		{
			return (_value);
		}

		CallbackContainer::Contract subscribe(std::function<void()> p_callback)
		{
			return (std::move(_callbacks.subscribe(p_callback)));
		}

		/**
		 * @brief Equality comparison operator.
		 *
		 * Compares this ObservableValue with another ObservableValue of potentially different type.
		 *
		 * @tparam TOtherType The type of the other ObservableValue to compare with.
		 * @param p_other The other ObservableValue to compare against.
		 * @return bool True if the values are equal, false otherwise.
		 */
		template<typename TOtherType>
		bool operator==(const ObservableValue<TOtherType>& p_other) const
		{
			return _value == p_other._value;
		}

		/**
		 * @brief Inequality comparison operator.
		 *
		 * @tparam TOtherType The type of the other ObservableValue to compare with.
		 * @param p_other The other ObservableValue to compare against.
		 * @return bool True if the values are not equal, false otherwise.
		 */
		template<typename TOtherType>
		bool operator!=(const ObservableValue<TOtherType>& p_other) const
		{
			return _value != p_other._value;
		}

		/**
		 * @brief Less than comparison operator.
		 *
		 * @tparam TOtherType The type of the other ObservableValue to compare with.
		 * @param p_other The other ObservableValue to compare against.
		 * @return bool True if this value is less than the other, false otherwise.
		 */
		template<typename TOtherType>
		bool operator<(const ObservableValue<TOtherType>& p_other) const
		{
			return _value < p_other._value;
		}

		/**
		 * @brief Greater than comparison operator.
		 *
		 * @tparam TOtherType The type of the other ObservableValue to compare with.
		 * @param p_other The other ObservableValue to compare against.
		 * @return bool True if this value is greater than the other, false otherwise.
		 */
		template<typename TOtherType>
		bool operator>(const ObservableValue<TOtherType>& p_other) const
		{
			return _value > p_other._value;
		}

		/**
		 * @brief Less than or equal to comparison operator.
		 *
		 * @tparam TOtherType The type of the other ObservableValue to compare with.
		 * @param p_other The other ObservableValue to compare against.
		 * @return bool True if this value is less than or equal to the other, false otherwise.
		 */
		template<typename TOtherType>
		bool operator<=(const ObservableValue<TOtherType>& p_other) const
		{
			return _value <= p_other._value;
		}

		/**
		 * @brief Greater than or equal to comparison operator.
		 *
		 * @tparam TOtherType The type of the other ObservableValue to compare with.
		 * @param p_other The other ObservableValue to compare against.
		 * @return bool True if this value is greater than or equal to the other, false otherwise.
		 */
		template<typename TOtherType>
		bool operator>=(const ObservableValue<TOtherType>& p_other) const
		{
			return _value >= p_other._value;
		}

		/**
		 * @brief Assignment operator.
		 *
		 * Assigns a new value to this ObservableValue and notifies observers if the value changes.
		 *
		 * @tparam TOtherType The type of the value being assigned.
		 * @param p_value The value to assign.
		 * @return ObservableValue& Reference to this ObservableValue.
		 */
		template<typename TOtherType>
		ObservableValue& operator = (const TOtherType& p_value)
		{
			if (_value != p_value)
			{
				_value = p_value;
				_callbacks.notify();
			}
			return *this;
		}

		/**
		 * @brief Addition operator.
		 *
		 * Creates a new ObservableValue representing the sum of this value and another value.
		 *
		 * @tparam TOtherType The type of the other value.
		 * @param p_other The value to add.
		 * @return ObservableValue A new ObservableValue with the sum.
		 */
		template<typename TOtherType>
		ObservableValue operator+(const TOtherType& p_other) const
		{
			return ObservableValue(_value + p_other);
		}

		/**
		 * @brief Addition assignment operator.
		 *
		 * Adds a value to this ObservableValue and notifies observers.
		 *
		 * @tparam TOtherType The type of the other value.
		 * @param p_other The value to add.
		 * @return ObservableValue& Reference to this ObservableValue.
		 */
		template<typename TOtherType>
		ObservableValue& operator+=(const TOtherType& p_other)
		{
			_value += p_other;
			_callbacks.notify();
			return *this;
		}

		/**
		 * @brief Subtraction operator.
		 *
		 * Creates a new ObservableValue representing the difference between this value and another value.
		 *
		 * @tparam TOtherType The type of the other value.
		 * @param p_other The value to subtract.
		 * @return ObservableValue A new ObservableValue with the difference.
		 */
		template<typename TOtherType>
		ObservableValue operator-(const TOtherType& p_other) const
		{
			return ObservableValue(_value - p_other);
		}

		/**
		 * @brief Subtraction assignment operator.
		 *
		 * Subtracts a value from this ObservableValue and notifies observers.
		 *
		 * @tparam TOtherType The type of the other value.
		 * @param p_other The value to subtract.
		 * @return ObservableValue& Reference to this ObservableValue.
		 */
		template<typename TOtherType>
		ObservableValue& operator-=(const TOtherType& p_other)
		{
			_value -= p_other;
			_callbacks.notify();
			return *this;
		}

		/**
		 * @brief Multiplication operator.
		 *
		 * Creates a new ObservableValue representing the product of this value and another value.
		 *
		 * @tparam TOtherType The type of the other value.
		 * @param p_other The value to multiply.
		 * @return ObservableValue A new ObservableValue with the product.
		 */
		template<typename TOtherType>
		ObservableValue operator*(const TOtherType& p_other) const
		{
			return ObservableValue(_value * p_other);
		}

		/**
		 * @brief Multiplication assignment operator.
		 *
		 * Multiplies this ObservableValue by another value and notifies observers.
		 *
		 * @tparam TOtherType The type of the other value.
		 * @param p_other The value to multiply.
		 * @return ObservableValue& Reference to this ObservableValue.
		 */
		template<typename TOtherType>
		ObservableValue& operator*=(const TOtherType& p_other)
		{
			_value *= p_other;
			_callbacks.notify();
			return *this;
		}

		/**
		 * @brief Division operator.
		 *
		 * Creates a new ObservableValue representing the quotient of this value divided by another value.
		 *
		 * @tparam TOtherType The type of the other value.
		 * @param p_other The value to divide by.
		 * @return ObservableValue A new ObservableValue with the quotient.
		 */
		template<typename TOtherType>
		ObservableValue operator/(const TOtherType& p_other) const
		{
			return ObservableValue(_value / p_other);
		}

		/**
		 * @brief Division assignment operator.
		 *
		 * Divides this ObservableValue by another value and notifies observers.
		 *
		 * @tparam TOtherType The type of the other value.
		 * @param p_other The value to divide by.
		 * @return ObservableValue& Reference to this ObservableValue.
		 */
		template<typename TOtherType>
		ObservableValue& operator/=(const TOtherType& p_other)
		{
			_value /= p_other;
			_callbacks.notify();
			return *this;
		}

		void notify()
		{
			_callbacks.notify();
		}
	};
}