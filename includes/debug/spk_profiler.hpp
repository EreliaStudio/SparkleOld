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

	class Profiler : public spk::Singleton<Profiler>
	{
		friend class spk::Singleton<Profiler>;
		friend class ProfilerModule;

	private:
		template <typename TMetricType>
		class IMetric
		{
			friend class Profiler;
		protected:
			std::vector<TMetricType> _values;
			TMetricType _min;
			TMetricType _max;
			TMetricType _average;

			void addValue(const TMetricType& p_value)
			{
				_values.push_back(p_value);
			}

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
		};

	public:
		class TimeConsuptionMetric : public IMetric<long long>
		{
		public:
			long long executionDuration;

		private:
			spk::Chronometer _chrono;
			double _cpuUsage;

		public:
			TimeConsuptionMetric();
			void compute(const long long& p_totalProgramDuration);

			bool isStarted() const;
			void start();
			void stop();

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

		class CounterMetric : public IMetric<size_t>
		{
		private:
			size_t _value;

		public:
			CounterMetric();

			void trigger();
			void save();
			void insert(const size_t& p_value);
			void reset();
			const size_t& value() const;
			void set(size_t p_newValue);

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
		long long _startProgramTime;

		std::map<std::thread::id, std::wstring> _threadNames;
		std::map<std::thread::id, std::map<std::wstring, TimeConsuptionMetric>> _timeConsuptionMetrics;
		std::map<std::thread::id, std::map<std::wstring, CounterMetric>> _counterMetrics;

		CounterMetric* _fpsCounterPointer = nullptr;
		CounterMetric* _upsCounterPointer = nullptr;
		
		void _outputTimeConsuptionMetrics(const std::map<std::wstring, TimeConsuptionMetric>& p_timeMetrics);
		void _outputCounterMetricMetrics(const std::map<std::wstring, CounterMetric>& p_counterMetrics);
		void _printThreadMetrics(const std::thread::id& p_threadID);
		void computeResults();

	public:
		Profiler();
		~Profiler();

		void defineThreadName(const std::wstring& p_threadName);
		
		TimeConsuptionMetric& timeConsumptionMetric(const std::wstring& p_metricName);
		CounterMetric& counterMetric(const std::wstring& p_metricName);

		CounterMetric& fpsCounter();
		CounterMetric& upsCounter();
	};
}
