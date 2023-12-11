#include "debug/spk_profiler.hpp"
#include <numeric>
#include "spk_basic_functions.hpp"

namespace spk
{
	Profiler::TimeConsuptionMetric::TimeConsuptionMetric()
	{

	}

	void Profiler::TimeConsuptionMetric::compute(const long long& p_totalProgramDuration)
	{
		IMetric<long long>::compute();
		_cpuUsage = static_cast<double>(std::reduce(_values.begin(), _values.end())) / static_cast<double>(p_totalProgramDuration) * 100.0;
	}

	bool Profiler::TimeConsuptionMetric::isStarted() const
	{
		return (_chrono.isRunning());
	}

	void Profiler::TimeConsuptionMetric::start()
	{
		_chrono.start();
	}

	void Profiler::TimeConsuptionMetric::stop()
	{
		addValue(_chrono.stop());
	}

	Profiler::CounterMetric::CounterMetric()
	{
		_value = 0;
	}

	void Profiler::CounterMetric::trigger()
	{
		_value++;
	}

	void Profiler::CounterMetric::save()
	{
		addValue(_value);
	}

	void Profiler::CounterMetric::insert(const size_t& p_value)
	{
		addValue(p_value);
	}

	void Profiler::CounterMetric::reset()
	{
		save();
		_value = 0;
	}

	const size_t& Profiler::CounterMetric::value() const
	{
		return (_value);
	}

	void Profiler::CounterMetric::set(size_t p_newValue)
	{
		_value = p_newValue;
	}

	void Profiler::_outputTimeConsuptionMetrics(const std::map<std::wstring, TimeConsuptionMetric>& p_timeMetrics)
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

	void Profiler::_outputCounterMetricMetrics(const std::map<std::wstring, CounterMetric>& p_counterMetrics)
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

	void Profiler::_printThreadMetrics(const std::thread::id& p_threadID)
	{
		spk::cout << "Thread ID [" << _threadNames[p_threadID] << "]" << std::endl;
		_outputTimeConsuptionMetrics(_timeConsuptionMetrics[p_threadID]);
		_outputCounterMetricMetrics(_counterMetrics[p_threadID]);
	}

	void Profiler::computeResults()
	{
		long long programTotalDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - _startProgramTime;

		for (auto& threadEntry : _timeConsuptionMetrics)
		{
			for (auto& metricEntry : threadEntry.second)
			{
				metricEntry.second.compute(programTotalDuration);
			}
		}

		for (auto& threadEntry : _counterMetrics)
		{
			for (auto& metricEntry : threadEntry.second)
			{
				metricEntry.second.compute();
			}
		}
	}

	Profiler::Profiler() :
		_startProgramTime(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
	{

	}

	Profiler::~Profiler()
	{
		computeResults();
		for (auto& threadInfo : _threadNames)
		{
			_printThreadMetrics(threadInfo.first);
		}
	}

	void Profiler::defineThreadName(const std::wstring& p_threadName)
	{
		_threadNames[std::this_thread::get_id()] = p_threadName;
	}

	Profiler::TimeConsuptionMetric& Profiler::timeConsumptionMetric(const std::wstring& p_metricName)
	{
		std::scoped_lock<std::recursive_mutex> lock(_mutex);
		if (_threadNames.contains(std::this_thread::get_id()) == false)
			_threadNames[std::this_thread::get_id()] = L"Unnamed";

		if (_timeConsuptionMetrics[std::this_thread::get_id()].contains(p_metricName) == false)
			_timeConsuptionMetrics[std::this_thread::get_id()][p_metricName] = TimeConsuptionMetric();

		return (_timeConsuptionMetrics[std::this_thread::get_id()][p_metricName]);
	}
	
	bool Profiler::containTimeConsumptionMetric(const std::wstring& p_metricName) const
	{
		std::scoped_lock<std::recursive_mutex> lock(_mutex);
		if (_threadNames.contains(std::this_thread::get_id()) == false ||
			_timeConsuptionMetrics.at(std::this_thread::get_id()).contains(p_metricName) == false)
			return (false);
		return (true);
	}
	
	Profiler::CounterMetric& Profiler::counterMetric(const std::wstring& p_metricName)
	{
		std::scoped_lock<std::recursive_mutex> lock(_mutex);
		if (_threadNames.contains(std::this_thread::get_id()) == false)
			_threadNames[std::this_thread::get_id()] = L"Unnamed";

		if (_counterMetrics[std::this_thread::get_id()].contains(p_metricName) == false)
		{
			_counterMetrics[std::this_thread::get_id()][p_metricName] = CounterMetric();
		}

		return (_counterMetrics[std::this_thread::get_id()][p_metricName]);
	}
	
	bool Profiler::containCounterMetric(const std::wstring& p_metricName) const
	{
		std::scoped_lock<std::recursive_mutex> lock(_mutex);
		if (_threadNames.contains(std::this_thread::get_id()) == false ||
			_counterMetrics.at(std::this_thread::get_id()).contains(p_metricName) == false)
		{
			return (false);
		}
		return (true);
	}
	
	Profiler::CounterMetric& Profiler::fpsCounter()
	{
		if (_fpsCounterPointer == nullptr)
		{
			_fpsCounterPointer = &(counterMetric(FPS_COUNTER_NAME));
		}
		return (*_fpsCounterPointer);
	}

	Profiler::CounterMetric& Profiler::upsCounter()
	{
		if (_upsCounterPointer == nullptr)
		{
			_upsCounterPointer = &(counterMetric(UPS_COUNTER_NAME));
		}
		return (*_upsCounterPointer);
	}
}
