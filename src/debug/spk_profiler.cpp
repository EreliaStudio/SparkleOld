#include "debug/spk_profiler.hpp"

namespace spk
{
	Profiler::Profiler()
	{
		if (spk::Singleton<spk::TimeMetrics>::instance() == nullptr)
		{
			throw std::runtime_error("Profiler can't be launched without an application");
		}
	}

	const spk::Chronometer &Profiler::chronometer(const std::wstring &p_key) const
	{
		if (_chronometers.contains(p_key) == false)
		{
			throw std::runtime_error("This Chronometer does not exist ");
		}
		return (_chronometers.at(p_key));
	}

	void Profiler::resetChronometer(const std::wstring &p_key)
	{
		if (_chronometers.contains(p_key) == false)
		{
			throw std::runtime_error("This Chronometer does not exist ");
		}
		_chronometers[p_key].reset();
	}

	void Profiler::startChronometer(const std::wstring &p_key)
	{
		_chronometers[p_key].start();
	}

	void Profiler::resumeChronometer(const std::wstring &p_key)
	{
		if (_chronometers.contains(p_key) == false)
		{
			throw std::runtime_error("This Chronometer does not exist ");
		}
		_chronometers[p_key].resume();
	}

	const long long Profiler::stopChronometer(const std::wstring &p_key)
	{
		if (_chronometers.contains(p_key) == false)
		{
			throw std::runtime_error("This Chronometer does not exist ");
		}
		return (_chronometers[p_key].stop());
	}

	void Profiler::createCounter(const std::wstring& p_key)
	{
		if (_counters.contains(p_key) == true)
			throw std::runtime_error("Counter [] already exist");
		_counters[p_key] = 0;
	}

	void Profiler::increaseCounter(const std::wstring& p_key)
	{
		if (_counters.contains(p_key) == false)
			createCounter(p_key);
		_counters[p_key]++;
	}
	
	void Profiler::setCounter(const std::wstring& p_key, const size_t& p_value)
	{
		_counters[p_key] = p_value;
	}

	void Profiler::resetCounter(const std::wstring& p_key)
	{
		if (_counters.contains(p_key) == false)
			throw std::runtime_error("Counter [] don't exist");
		_counters[p_key]++;
	}
	
	bool Profiler::containCounter(const std::wstring& p_key) const
	{
		return (_counters.contains(p_key));
	}

	const size_t& Profiler::counter(const std::wstring& p_key) const
	{
		if (_counters.contains(p_key) == false)
			throw std::runtime_error("Counter [] don't exist");
		return (_counters.at(p_key));
	}
	
}
