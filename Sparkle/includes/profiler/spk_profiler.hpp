#pragma once

#include "data_structure/spk_safe_pointer.hpp"

#include "miscellaneous/JSON/spk_JSON_object.hpp"

#include "profiler/spk_counter_metrics.hpp"
#include "profiler/spk_time_consumption_metrics.hpp"

namespace spk
{
	/**
     * @brief A class to profile and track various metrics in the application.
     * 
     * Profiler manages a collection of metrics, allowing for the creation,
     * retrieval, and reporting of performance data.
     */
	class Profiler
	{
	private:
		std::map<std::wstring, AbstractMetrics*> _metrics; ///< Map of metric names to AbstractMetrics pointers.

	public:
        /**
         * @brief Constructs a Profiler instance.
         */
		Profiler();

	    /**
         * @brief Destructs the Profiler instance.
         */
		~Profiler();

        /**
         * @brief Retrieves or creates a metric of a specified type.
         * 
         * If a metric with the given name exists, it is returned. Otherwise, a new metric
         * of type TMetricsType is created with the provided arguments and returned.
         * 
         * @tparam TMetricsType The type of the metrics to retrieve or create.
         * @tparam Args Variadic template for constructor arguments of TMetricsType.
         * @param p_metricsName The name of the metric to retrieve or create.
         * @param p_args Arguments to be forwarded to the constructor of TMetricsType.
         * @return Reference to the retrieved or created metric of type TMetricsType.
         */
		template <typename TMetricsType, typename ... Args>
		TMetricsType& metrics(const std::wstring& p_metricsName, Args&& ... p_args)
		{
			if (_metrics.contains(p_metricsName) == true)
				return (*(static_cast<TMetricsType*>(_metrics[p_metricsName])));

			TMetricsType* result = new TMetricsType(std::forward<Args>(p_args)...);

			_metrics[p_metricsName] = result;

			return (*result);
		}
		
        /**
         * @brief Generates a JSON object containing all the metrics reports.
         * @return JSON::Object representing the aggregated metrics reports.
         */
		spk::JSON::Object emitReport() const;
	};
}