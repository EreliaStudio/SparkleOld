#pragma once

#include "data_structure/spk_data_buffer.hpp"
#include <vector>

namespace spk
{
	/**
	 * @class Memento
	 * @brief A class representing a memento in the Memento design pattern.
	 *
	 * This class is used to store the state of an object so that it can be restored later.
	 */
	class Memento
	{
	public:
		using Snapshot = spk::DataBuffer; ///< The data container used for storing snapshot datas.

	private:
		std::vector<Snapshot> _snapshots; ///< List of stored states (snapshots)
		int _index = -1;				  ///< Current index of the snapshot

		/**
		 * @brief Save the current state into a snapshot.
		 *
		 * This is a pure virtual function, so it needs to be implemented in the derived classes.
		 *
		 * @return A snapshot representing the current state.
		 */

		virtual Snapshot _onSave() = 0;

		/**
		 * @brief Load a state from a snapshot.
		 *
		 * This is a pure virtual function, so it needs to be implemented in the derived classes.
		 *
		 * @param p_snapshot The snapshot to load the state from.
		 */
		virtual void _onLoad(const Snapshot &p_snapshot) = 0;

		/**
		 * @brief Internal function to load a state.
		 *
		 * This function resets the current snapshot and then loads a state from it.
		 */
		void _load()
		{
			_snapshots[_index].reset();
			_onLoad(_snapshots[_index]);
		}

	public:
		/**
		 * @brief Save the current state.
		 *
		 * This function saves the current state into a snapshot and stores it in the list.
		 */
		void save()
		{
			if (static_cast<size_t>(_index + 1) < _snapshots.size())
			{
				_snapshots.erase(_snapshots.begin() + _index + 1, _snapshots.end());
			}
			_snapshots.push_back(_onSave());
			_index++;
		}

		/**
		 * @brief Undo the last operation.
		 *
		 * This function undoes the last operation and restores the previous state.
		 *
		 * @throws std::runtime_error If there are no more snapshots to undo.
		 */
		void undo()
		{
			if (_index < 0)
				throw std::runtime_error("Can't undo cause no snapshot left");
			_index--;
			_load();
		}

		/**
		 * @brief Redo the last undone operation.
		 *
		 * This function redoes the last undone operation and restores the next state.
		 *
		 * @throws std::runtime_error If there are no more snapshots to redo.
		 */
		void redo()
		{
			if (static_cast<size_t>(_index + 1) >= _snapshots.size())
				throw std::runtime_error("Can't redo cause no snapshot left");
			_index++;
			_load();
		}
	};

	/**
	 * @class StandardType
	 * @brief A class template for standard layout types, derived from Memento.
	 *
	 * This class is used to save and load the state of a standard layout type.
	 *
	 * @tparam TType The type of the standard layout type.
	 */
	template <typename TType, typename = std::enable_if_t<std::is_standard_layout<TType>::value>>
	struct StandardType : Memento
	{
		TType _value; ///< The value of the standard layout type.

		/**
		 * @brief Save the current state into a snapshot.
		 *
		 * This function saves the current value into a snapshot.
		 *
		 * @return A snapshot representing the current value.
		 */
		Snapshot _onSave()
		{
			Snapshot result;

			result << _value;

			return result;
		}

		/**
		 * @brief Load a state from a snapshot.
		 *
		 * This function loads a value from a snapshot.
		 *
		 * @param p_snapshot The snapshot to load the value from.
		 */
		void _onLoad(const Snapshot &p_snapshot)
		{
			p_snapshot >> _value;
		}

		/**
		 * @brief Default constructor.
		 *
		 * This constructor initializes the value to its default state.
		 */
		StandardType() : _value()
		{
		}

		/**
		 * @brief Parameterized constructor.
		 *
		 * This constructor initializes the value with a given value.
		 *
		 * @param p_value The initial value.
		 */
		StandardType(const TType &p_value) : _value(p_value)
		{
		}

		/**
		 * @brief Assignment operator.
		 *
		 * This operator assigns a new value to the object and returns a reference to the object.
		 *
		 * @param p_value The new value.
		 * @return A reference to the object.
		 */
		StandardType &operator=(const TType &p_value)
		{
			_value = p_value;
			return (*this);
		}

		/**
		 * @brief Type conversion operator.
		 *
		 * This operator returns a reference to the value.
		 *
		 * @return A reference to the value.
		 */
		operator TType &()
		{
			return (_value);
		}

		/**
		 * @brief Type conversion operator.
		 *
		 * This operator returns a const reference to the value.
		 *
		 * @return A const reference to the value.
		 */
		operator const TType &() const
		{
			return (_value);
		}
	};
}