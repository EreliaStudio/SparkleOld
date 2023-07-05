#pragma once

#include <string>
#include <map>
#include <stdexcept>

#include "system/spk_chronometer.hpp"
#include "design_pattern/spk_singleton.hpp"

namespace spk
{
	namespace Debug
	{
		class Profiler : public spk::Singleton<Profiler>
		{
			friend class spk::Singleton<Profiler>;

		private:
			std::map<std::wstring, spk::Chronometer> _chronometers;

			Profiler()
			{
				if (spk::Singleton<spk::TimeMetrics>::instance() == nullptr)
				{
					throw std::runtime_error("Profiler can't be launched without an application");
				}
			}

		public:

			const spk::Chronometer &chronometer(const std::wstring &p_name) const
			{
				if (_chronometers.count(p_name) == 0)
				{
					throw std::runtime_error("This Chronometer does not exist ");
				}
				return (_chronometers.at(p_name));
			}

			void resetChronometer(const std::wstring &p_name)
			{
				if (_chronometers.count(p_name) == 0)
				{
					throw std::runtime_error("This Chronometer does not exist ");
				}
				_chronometers[p_name].reset();
			}

			void startChronometer(const std::wstring &p_name)
			{
				_chronometers[p_name].start();
			}

			void resumeChronometer(const std::wstring &p_name)
			{
				if (_chronometers.count(p_name) == 0)
				{
					throw std::runtime_error("This Chronometer does not exist ");
				}
				_chronometers[p_name].resume();
			}

			const long long stopChronometer(const std::wstring &p_name)
			{
				if (_chronometers.count(p_name) == 0)
				{
					throw std::runtime_error("This Chronometer does not exist ");
				}
				return (_chronometers[p_name].stop());
			}
		};
	}
}
