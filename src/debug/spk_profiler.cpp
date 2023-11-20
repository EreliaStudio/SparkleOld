#include "debug/spk_profiler.hpp"
#include <numeric>
#include "spk_basic_functions.hpp"

namespace spk
{
	void Profiler::ChronometerResult::compute()
	{
		if (durations.empty()) {
            max = min = average = 0;
            return;
        }

		for (size_t i = 0; i < durations.size(); i++)
		{
			if (i != 0)
				spk::cout << " - ";
			spk::cout << durations[i];
		}
		spk::cout << std::endl;

        max = *std::max_element(durations.begin(), durations.end());
        min = *std::min_element(durations.begin(), durations.end());

        long long sum = std::accumulate(durations.begin(), durations.end(), 0LL);
        average = sum / durations.size();
	}

	Profiler::Profiler()
	{
		if (spk::Singleton<spk::TimeMetrics>::instance() == nullptr)
		{
			throw std::runtime_error("Profiler can't be launched without an application");
		}
	}

	Profiler::~Profiler()
	{
		for (const auto& counter : _counters)
        {
            spk::cout << "Counter [" << counter.first << "] - " << counter.second << " trigger(s)" << std::endl;
        }

		for (auto& chrono : _chronometers)
		{
			if (chrono.second.isRunning() == true)
				stopChronometer(chrono.first);
		}

		for (auto& chronoResult : _chronometerResults)
		{
			chronoResult.second.compute();

			spk::cout << "Chronometer [" << chronoResult.first << "]:" << std::endl;
			spk::cout << "  - Length  : " << chronoResult.second.durations.size() << " values" << std::endl;
			spk::cout << "  - Min     : " << chronoResult.second.min << " microseconds" << std::endl;
			spk::cout << "  - Max     : " << chronoResult.second.max << " microseconds" << std::endl;
			spk::cout << "  - Average : " << chronoResult.second.average << " microseconds" << std::endl;
		}
	}

	void Profiler::createChronometer(const std::wstring &p_key)
	{
		if (_chronometers.contains(p_key) == true)
		{
			throw std::runtime_error("The chronometer [" + spk::wstringToString(p_key) + "] already exist ");
		}
		_chronometers[p_key] = spk::Chronometer();
	}

	const spk::Chronometer &Profiler::chronometer(const std::wstring &p_key) const
	{
		if (_chronometers.contains(p_key) == false)
		{
			throw std::runtime_error("The chronometer [" + spk::wstringToString(p_key) + "] does not exist ");
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
		long long result = _chronometers[p_key].stop();
		spk::cout << "Duration : " << result << std::endl;
		_chronometerResults[p_key].durations.push_back(result);
		return (result);
	}

	const std::map<std::wstring, size_t>& Profiler::counters() const
	{
		return (_counters);
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
	
	const size_t& Profiler::FPS() const
	{
		return (_normalizedFPS);
	}
	
	const size_t& Profiler::UPS() const
	{
		return (_normalizedUPS);
	}
}
