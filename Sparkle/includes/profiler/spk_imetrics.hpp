#pragma once

#include <cstddef>
#include "miscellaneous/JSON/spk_JSON_object.hpp"

namespace spk
{
	class AbstractMetrics
	{
	private:

	public:
		virtual spk::JSON::Object emitReport() const = 0;
	};

	template<typename TType>
	class IMetrics : public AbstractMetrics
	{
	private:
		TType _min = TType();
		TType _max = TType();
		TType _average = TType();
		size_t _nbMetrics = 0;

	protected:
		void _insertValue(const TType& p_value)
		{
			if (_nbMetrics == 0 || _min > p_value)
				_min = p_value;
				
			if (_nbMetrics == 0 || _max < p_value)
				_max = p_value;

			_average = (_average * _nbMetrics + p_value);
			_nbMetrics++;
			_average /= _nbMetrics;
		}

	public:
		IMetrics()
		{

		}

		const TType& min() const
		{
			return (_min);
		}

		const TType& max() const
		{
			return (_max);
		}

		const TType& average() const
		{
			return (_average);
		}
		
		const size_t& cardinal() const
		{
			return (_nbMetrics);
		}
	};
}