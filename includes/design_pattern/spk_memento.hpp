#pragma once

#include "data_structure/spk_data_buffer.hpp"
#include <vector>

namespace spk
{
	template <typename TType>
	class Memento
	{
	public:
		using Snapshot = spk::DataBuffer;

	private:
		std::vector<Snapshot> _states;
		int _currentState = -1;

		void _load(Snapshot& p_snapshot, TType& p_state)
		{
			p_snapshot >> p_state;
			p_snapshot.reset();
		}

	public:
		/**
		 * @brief Save the current state of p_state.
		 *
		 * @param p_state Value to save.
		 * @note If there is a state after the current one, it will be erased.
		 */
		void save(const TType& p_state)
		{
			if (_currentState != _states.size() - 1)
				_states.erase(_states.begin() + _currentState + 1, _states.end());

			Snapshot newSnapshot;
			newSnapshot << p_state;
			_states.push_back(newSnapshot);
			_currentState = _states.size() - 1;
		}

		/**
		 * @brief Set p_state to the previous saved one.
		 *
		 * @param p_state The state to set that has been saved.
		 * @throw std::runtime_error If there is no more state to undo.
		 */
		void undo(TType& p_state)
		{
			if (_currentState <= 0)
				throw std::runtime_error("Memento: No more _states to undo");

			_currentState--;
			_load(_states[_currentState], p_state);
		}

		/**
		 * @brief Set p_state to the next saved one.
		 *
		 * @param p_state The state to set that has been saved.
		 * @throw std::runtime_error If there is no more state to redo.
		 */
		void redo(TType& p_state)
		{
			if (_currentState >= _states.size() - 1)
				throw std::runtime_error("Memento: No more _states to redo");

			_currentState++;
			_load(_states[_currentState], p_state);
		}

		/**
		 * @brief Reset the memento and erase all saved states.
		 */
		void reset()
		{
			_states.clear();
			_currentState = -1;
		}
	};
}