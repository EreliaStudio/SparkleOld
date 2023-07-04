#pragma once

#include "data_structure/spk_data_buffer.hpp"
#include <vector>

namespace spk
{
	class Memento
	{
	public:
		using Snapshot = spk::DataBuffer;

	private:
		std::vector<Snapshot> _snapshots;
		int _index = -1;

		virtual Snapshot _onSave() = 0;
		virtual void _onLoad(const Snapshot& p_snapshot) = 0;

		void _load()
		{ 
			_snapshots[_index].reset();
			_onLoad(_snapshots[_index]);
		}

	public:
		void save()
		{
			if (_index + 1< _snapshots.size())
			{
				_snapshots.erase(_snapshots.begin() + _index + 1, _snapshots.end());
			}
			_snapshots.push_back(_onSave());
			_index++;
		}

		void undo()
		{
			if (_index < 0)
				throw std::runtime_error("Can't undo cause no snapshot left");
			_index--;
			_load();
		}

		void redo()
		{
			if ((_index + 1) >= _snapshots.size())
				throw std::runtime_error("Can't redo cause no snapshot left");
			_index++;
			_load();
		}
	};

	template <typename TType, typename = std::enable_if_t<std::is_standard_layout<TType>::value>>
	struct StandardType : Memento
	{
		TType _value;

		Snapshot _onSave()
		{
			Snapshot result;

			result << _value;

			return result;
		}
		
		void _onLoad(const Snapshot& p_snapshot)
		{
			p_snapshot >> _value;
		}

		StandardType() :
			_value()
		{

		}

		StandardType(const TType& p_value) :
			_value(p_value)	 
		{

		}
		
		StandardType& operator = (const TType& p_value)
		{
			_value = p_value;
			return (*this);
		}

		operator TType&()
		{
			return (_value);
		}
		
		operator const TType&() const
		{
			return (_value);
		}
	};
}