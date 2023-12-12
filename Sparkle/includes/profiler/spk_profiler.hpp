#pragma once

#include "data_structure/spk_safe_pointer.hpp"

#include "miscellaneous/JSON/spk_JSON_object.hpp"

#include "profiler/spk_counter_metrics.hpp"
#include "profiler/spk_time_consumption_metrics.hpp"

namespace spk
{
	class Profiler
	{
	private:
		std::map<std::wstring, AbstractMetrics*> _metrics;

	public:
		Profiler();
		~Profiler();

		template <typename TMetricsType, typename ... Args>
		TMetricsType& metrics(const std::wstring& p_metricsName, Args&& ... p_args)
		{
			if (_metrics.contains(p_metricsName) == true)
				return (*(static_cast<TMetricsType*>(_metrics[p_metricsName])));

			TMetricsType* result = new TMetricsType(std::forward<Args>(p_args)...);

			_metrics[p_metricsName] = result;

			return (*result);
		}

		spk::JSON::Object emitReport() const;
	};
}