#pragma once

#include <string>
#include <map>
#include <stdexcept>
#include <numeric>

#include "system/spk_chronometer.hpp"
#include "design_pattern/spk_singleton.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	class ProfilerModule;

    /** 
     * @class Profiler
     * @brief A class for profiling various metrics like time consumption and counters.
     */
	class Profiler : public spk::Singleton<Profiler>
	{
		friend class spk::Singleton<Profiler>;
		friend class ProfilerModule;

	private:
		/** 
         * @class IMetric
         * @brief Template class for handling metric data.
         */
        template <typename TMetricType>
        class IMetric
        {
            friend class Profiler;
        protected:
            std::vector<TMetricType> _values; ///< Vector holding the metric values.
            TMetricType _min;                 ///< Minimum value of the metric.
            TMetricType _max;                 ///< Maximum value of the metric.
            TMetricType _average;             ///< Average value of the metric.

            /** 
             * @brief Add a new value to the metric.
             * @param p_value Value to be added to the metric.
             */
            void addValue(const TMetricType& p_value)
			{
				_values.push_back(p_value);
			}

            /** 
             * @brief Compute the min, max, and average values of the metric.
             */
            void compute()
			{
				if (_values.empty() == true)
				{
            		_max = _min = _average = 0;
            		return;
        		}

				_min = *(std::min_element(_values.begin(), _values.end()));
				_max = *(std::max_element(_values.begin(), _values.end()));
				_average = (_values.empty()? TMetricType() : std::accumulate(_values.begin(), _values.end(), TMetricType())) / _values.size();
			}

        public:
            // Public members can be added here.
        };

	public:
		        /** 
         * @class TimeConsuptionMetric
         * @brief Metric class for tracking time consumption.
         */
        class TimeConsuptionMetric : public IMetric<long long>
        {
        public:
            long long executionDuration; ///< Duration of the execution.

        private:
            spk::Chronometer _chrono;    ///< Chronometer instance for measuring time.
            double _cpuUsage;            ///< CPU usage percentage.

        public:
            TimeConsuptionMetric();
            
            /** 
             * @brief Compute the time consumption metric.
             * @param p_totalProgramDuration Total duration of the program.
             */
            void compute(const long long& p_totalProgramDuration);

            /** 
             * @brief Check if the metric has started.
             * @return True if started, false otherwise.
             */
            bool isStarted() const;

            /** 
             * @brief Start the metric computation.
             */
            void start();

            /** 
             * @brief Stop the metric computation.
             */
            void stop();

            /** 
             * @brief Overload the output stream operator for TimeConsuptionMetric.
             * @param p_os Output stream.
             * @param p_counter Time consumption metric to be outputted.
             * @return Reference to the output stream.
             */
			friend std::wostream& operator << (std::wostream& p_os, const TimeConsuptionMetric& p_counter)
			{
				p_os << "    - Min         : " << p_counter._min << std::endl;
				p_os << "    - Max         : " << p_counter._max << std::endl;
				p_os << "    - Average     : " << p_counter._average << std::endl;
				p_os << "    - CPU usage   : " << p_counter._cpuUsage << " %" << std::endl;
				p_os << "    - Cardinality : " << p_counter._values.size() << std::endl;
				return (p_os);
			}
		};

		/** 
         * @class CounterMetric
         * @brief Metric class for tracking counters.
         */
        class CounterMetric : public IMetric<size_t>
        {
        private:
            size_t _value; ///< Current value of the counter.

        public:
            CounterMetric();

            /** 
             * @brief Trigger the counter.
             */
            void trigger();

            /** 
             * @brief Save the current value of the counter.
             */
            void save();

            /** 
             * @brief Insert a new value into the counter.
             * @param p_value Value to be inserted.
             */
            void insert(const size_t& p_value);

            /** 
             * @brief Reset the counter to its initial state.
             */
            void reset();

            /** 
             * @brief Get the current value of the counter.
             * @return The current value of the counter.
             */
            const size_t& value() const;

            /** 
             * @brief Set a new value for the counter.
             * @param p_newValue New value to be set.
             */
            void set(size_t p_newValue);

            /** 
             * @brief Overload the output stream operator for CounterMetric.
             * @param p_os Output stream.
             * @param p_counter Counter metric to be outputted.
             * @return Reference to the output stream.
             */
			friend std::wostream& operator << (std::wostream& p_os, const CounterMetric& p_counter)
			{
				p_os << "    - Min         : " << p_counter._min << " trigger(s)" << std::endl;
				p_os << "    - Max         : " << p_counter._max << " trigger(s)" << std::endl;
				p_os << "    - Average     : " << p_counter._average << " trigger(s)" << std::endl;
				p_os << "    - Cardinality : " << p_counter._values.size() << std::endl;
				return (p_os);
			}
		};
		static inline const std::wstring FPS_COUNTER_NAME = L"FPS";
		static inline const std::wstring UPS_COUNTER_NAME = L"UPS";

	private:
		long long _startProgramTime; ///< Timestamp marking the start of program execution.

		static inline std::recursive_mutex _mutex; ///< Mutex for thread-safe operations.
		std::map<std::thread::id, std::wstring> _threadNames; ///< Map of thread IDs to their names.
		std::map<std::thread::id, std::map<std::wstring, TimeConsuptionMetric>> _timeConsuptionMetrics; ///< Nested map for time consumption metrics organized by thread and metric name.
		std::map<std::thread::id, std::map<std::wstring, CounterMetric>> _counterMetrics; ///< Nested map for counter metrics organized by thread and metric name.

		CounterMetric* _fpsCounterPointer = nullptr; ///< Pointer to FPS (Frames Per Second) counter metric.
		CounterMetric* _upsCounterPointer = nullptr; ///< Pointer to UPS (Updates Per Second) counter metric.
		
		/** 
		 * @brief Outputs the time consumption metrics.
		 * @param p_timeMetrics Map of time consumption metrics to output.
		 */
		void _outputTimeConsuptionMetrics(const std::map<std::wstring, TimeConsuptionMetric>& p_timeMetrics);

		/** 
		 * @brief Outputs the counter metric metrics.
		 * @param p_counterMetrics Map of counter metrics to output.
		 */
		void _outputCounterMetricMetrics(const std::map<std::wstring, CounterMetric>& p_counterMetrics);

		/** 
		 * @brief Prints the metrics of a specific thread.
		 * @param p_threadID ID of the thread whose metrics are to be printed.
		 */
		void _printThreadMetrics(const std::thread::id& p_threadID);

		/** 
		 * @brief Computes and summarizes the results from all metrics.
		 */
		void computeResults();

	public:
		/** 
		 * @brief Constructor for the Profiler class.
		 */
		Profiler();

		/** 
		 * @brief Destructor for the Profiler class.
		 */
		~Profiler();

		/** 
		 * @brief Defines a name for the current thread.
		 * @param p_threadName The name to be assigned to the current thread.
		 */
		void defineThreadName(const std::wstring& p_threadName);
		
		/** 
		 * @brief Gets a reference to a time consumption metric by name.
		 * @param p_metricName The name of the time consumption metric.
		 * @return Reference to the TimeConsuptionMetric object.
		 */
		TimeConsuptionMetric& timeConsumptionMetric(const std::wstring& p_metricName);

		/** 
		 * @brief Checks if a time consumption metric exists.
		 * @param p_metricName The name of the time consumption metric to check.
		 * @return True if the metric exists, false otherwise.
		 */
		bool containTimeConsumptionMetric(const std::wstring& p_metricName) const;

		/** 
		 * @brief Gets a reference to a counter metric by name.
		 * @param p_metricName The name of the counter metric.
		 * @return Reference to the CounterMetric object.
		 */
		CounterMetric& counterMetric(const std::wstring& p_metricName);

		/** 
		 * @brief Checks if a counter metric exists.
		 * @param p_metricName The name of the counter metric to check.
		 * @return True if the metric exists, false otherwise.
		 */
		bool containCounterMetric(const std::wstring& p_metricName) const;

		/** 
		 * @brief Gets a reference to the FPS (Frames Per Second) counter metric.
		 * @return Reference to the FPS CounterMetric object.
		 */
		CounterMetric& fpsCounter();

		/** 
		 * @brief Gets a reference to the UPS (Updates Per Second) counter metric.
		 * @return Reference to the UPS CounterMetric object.
		 */
		CounterMetric& upsCounter();
	};
}
