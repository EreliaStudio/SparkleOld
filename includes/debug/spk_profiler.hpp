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
			TimeConsuptionMetric()
			{

			}

			void compute(const long long& p_totalProgramDuration)
			{
				IMetric<long long>::compute();
				_cpuUsage = static_cast<double>(std::reduce(_values.begin(), _values.end())) / static_cast<double>(p_totalProgramDuration) * 100.0;
			}

			bool isStarted() const
			{
				return (_chrono.hasBeenStarted());
			}

			void start()
			{
				_chrono.start();
			}

			void stop()
			{
				addValue(_chrono.stop());
			}

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
			CounterMetric()
			{
				_value = 0;
			}

			void trigger()
			{
				_value++;
			}

			void save()
			{
				addValue(_value);
			}

			void reset()
			{
				save();
				_value = 0;
			}

			friend std::wostream& operator << (std::wostream& p_os, const CounterMetric& p_counter)
			{
				p_os << "    - Min         : " << p_counter._min << std::endl;
				p_os << "    - Max         : " << p_counter._max << std::endl;
				p_os << "    - Average     : " << p_counter._average << std::endl;
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
		std::map<std::thread::id, std::map<std::wstring, CounterMetric>> _triggerMetrics;

		CounterMetric* _fpsCounterPointer = nullptr;
		CounterMetric* _upsCounterPointer = nullptr;
		
		void _outputTimeConsuptionMetrics(const std::map<std::wstring, TimeConsuptionMetric>& p_timeMetrics)
		{
			spk::cout << "  TimeComsuption :" << std::endl;
			if (p_timeMetrics.size() == 0)
				spk::cout << "  - No metrics" << std::endl;

			for (const auto& timeConsuptionMetric : p_timeMetrics)
			{
				spk::cout << "  - " << timeConsuptionMetric.first << std::endl;
				spk::cout << timeConsuptionMetric.second << std::endl;
			}
		}
		void _outputCounterMetricMetrics(const std::map<std::wstring, CounterMetric>& p_counterMetrics)
		{
			spk::cout << "  Counters :" << std::endl;
			if (p_counterMetrics.size() == 0)
				spk::cout << "  - No metrics" << std::endl;
			for (auto& threadCounterMetricMetrics : p_counterMetrics) 
			{
				spk::cout << "  - " << threadCounterMetricMetrics.first<< std::endl;
				spk::cout << threadCounterMetricMetrics.second << std::endl;
			}
		}

		void _printThreadMetrics(const std::thread::id& p_threadID)
		{
			spk::cout << "Thread ID [" << _threadNames[p_threadID] << "]" << std::endl;
			_outputTimeConsuptionMetrics(_timeConsuptionMetrics[p_threadID]);
			_outputCounterMetricMetrics(_triggerMetrics[p_threadID]);
		}

		void computeResults()
		{
			long long programTotalDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - _startProgramTime;

			for (auto& threadEntry : _timeConsuptionMetrics)
			{
				for (auto& metricEntry : threadEntry.second)
				{
					metricEntry.second.compute(programTotalDuration);
				}
			}

			for (auto& threadEntry : _triggerMetrics)
			{
				for (auto& metricEntry : threadEntry.second)
				{
					metricEntry.second.compute();
				}
			}
		}

	public:
		Profiler() :
			_startProgramTime(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
		{

		}

		~Profiler()
		{
			computeResults();
			for (auto& threadInfo : _threadNames)
			{
				_printThreadMetrics(threadInfo.first);
			}
		}

		void defineThreadName(const std::wstring& p_threadName)
		{
			_threadNames[std::this_thread::get_id()] = p_threadName;
		}

		TimeConsuptionMetric& createTimeConsumptionMetric(const std::wstring& p_metricName)
		{
			if (_threadNames.contains(std::this_thread::get_id()) == false)
				_threadNames[std::this_thread::get_id()] = L"Unnamed";

			if (_timeConsuptionMetrics[std::this_thread::get_id()].contains(p_metricName) == true)
				spk::throwException(L"TimeComsuptionMetric [" + p_metricName + L"] already exist");

			_timeConsuptionMetrics[std::this_thread::get_id()][p_metricName] = TimeConsuptionMetric();

			return (_timeConsuptionMetrics[std::this_thread::get_id()][p_metricName]);
		}
		
		CounterMetric& createCounterMetric(const std::wstring& p_metricName)
		{
			if (_threadNames.contains(std::this_thread::get_id()) == false)
				_threadNames[std::this_thread::get_id()] = L"Unnamed";

			if (_triggerMetrics[std::this_thread::get_id()].contains(p_metricName) == true)
				spk::throwException(L"CounterMetric [" + p_metricName + L"] already exist");

			_triggerMetrics[std::this_thread::get_id()][p_metricName] = CounterMetric();

			return (_triggerMetrics[std::this_thread::get_id()][p_metricName]);
		}

		TimeConsuptionMetric& timeConsumptionMetric(const std::wstring& p_metricName)
		{
			if (_timeConsuptionMetrics[std::this_thread::get_id()].contains(p_metricName) == false)
				spk::throwException(L"TimeComsuptionMetric [" + p_metricName + L"] doesn't exist");
			return (_timeConsuptionMetrics[std::this_thread::get_id()][p_metricName]);
		}

		CounterMetric& triggerMetric(const std::wstring& p_metricName)
		{
			if (_triggerMetrics[std::this_thread::get_id()].contains(p_metricName) == false)
				spk::throwException(L"CounterMetric [" + p_metricName + L"] doesn't exist");
			return (_triggerMetrics[std::this_thread::get_id()][p_metricName]);
		}

		CounterMetric& fpsCounter()
		{
			if (_fpsCounterPointer == nullptr)
			{
				DEBUG_LINE();
				_fpsCounterPointer = &(createCounterMetric(FPS_COUNTER_NAME));
			}
			return (*_fpsCounterPointer);
		}

		CounterMetric& upsCounter()
		{
			if (_upsCounterPointer == nullptr)
			{
				DEBUG_LINE();
				_upsCounterPointer = &(createCounterMetric(UPS_COUNTER_NAME));
			}
			return (*_upsCounterPointer);
		}
	};
}
