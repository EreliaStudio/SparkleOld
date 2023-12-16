#pragma once

#include <cstddef>
#include "miscellaneous/JSON/spk_JSON_object.hpp"

namespace spk
{
    /**
     * @brief Abstract base class for metric reporting.
     */
	class AbstractMetrics
	{
	private:

	public:

        /**
         * @brief Indicate if the counter need to be added to any report
        */
		virtual bool needEmition() const = 0;

        /**
         * @brief Virtual function to emit a report as a JSON object.
         * @return JSON::Object representing the metrics report.
         */
		virtual spk::JSON::Object emitReport() const = 0;
	};

    /**
     * @brief Template class for tracking and reporting statistics (min, max, average).
     * @tparam TType Data type of the metrics.
     */
	template<typename TType>
	class IMetrics : public AbstractMetrics
	{
	private:
		TType _min;    ///< Minimum value of the metrics.
        TType _max;    ///< Maximum value of the metrics.
        TType _average;///< Average of the metrics.
        size_t _nbMetrics; ///< Number of metric entries.

	protected:
		/**
         * @brief Updates the metrics with a new value.
         * @param p_value New value to be added to the metrics.
         */
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
	    /**
         * @brief Constructor for IMetrics. Initializes metrics to default values.
         */
		IMetrics() :
			_min(TType()),
			_max(TType()),
			_average(TType()),
			_nbMetrics(0)
		{

		}

        /**
         * @brief Gets the minimum metric value.
         * @return Minimum value.
         */
		const TType& min() const
		{
			return (_min);
		}

        /**
         * @brief Gets the maximum metric value.
         * @return Maximum value.
         */
		const TType& max() const
		{
			return (_max);
		}

        /**
         * @brief Gets the average metric value.
         * @return Average value.
         */
		const TType& average() const
		{
			return (_average);
		}
		
        /**
         * @brief Gets the number of metric entries.
         * @return Number of entries.
         */
		const size_t& cardinal() const
		{
			return (_nbMetrics);
		}
	};
}