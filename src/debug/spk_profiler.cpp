#include "debug/spk_profiler.hpp"

namespace spk
{
	namespace Debug
	{
		Profiler::Profiler()
		{
			if (spk::Singleton<spk::TimeMetrics>::instance() == nullptr)
			{
				throw std::runtime_error("Profiler can't be launched without an application");
			}
		}

		const spk::Chronometer &Profiler::chronometer(const std::wstring &p_name) const
		{
			if (_chronometers.count(p_name) == 0)
			{
				throw std::runtime_error("This Chronometer does not exist ");
			}
			return (_chronometers.at(p_name));
		}

		void Profiler::resetChronometer(const std::wstring &p_name)
		{
			if (_chronometers.count(p_name) == 0)
			{
				throw std::runtime_error("This Chronometer does not exist ");
			}
			_chronometers[p_name].reset();
		}

		void Profiler::startChronometer(const std::wstring &p_name)
		{
			_chronometers[p_name].start();
		}

		void Profiler::resumeChronometer(const std::wstring &p_name)
		{
			if (_chronometers.count(p_name) == 0)
			{
				throw std::runtime_error("This Chronometer does not exist ");
			}
			_chronometers[p_name].resume();
		}

		const long long Profiler::stopChronometer(const std::wstring &p_name)
		{
			if (_chronometers.count(p_name) == 0)
			{
				throw std::runtime_error("This Chronometer does not exist ");
			}
			return (_chronometers[p_name].stop());
		}
	}
}
