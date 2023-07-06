#pragma once

#include <string>
#include <map>
#include <stdexcept>

#include "system/spk_chronometer.hpp"
#include "design_pattern/spk_singleton.hpp"

namespace spk
{
	/**
	 * \class Profiler
	 * \brief A singleton class that handles profiling using various chronometers.
	 *
	 * This class is responsible for managing multiple chronometers, allowing the software
	 * to keep track of time metrics for profiling purposes. As a singleton, only one instance
	 * of this class can exist. This ensures consistency across the entire software.
	 */
	class Profiler : public spk::Singleton<Profiler>
	{
		friend class spk::Singleton<Profiler>;
	public:
		static inline const std::wstring RENDER_IPS_COUNTER = L"RenderIPS";
		static inline const std::wstring UPDATE_IPS_COUNTER = L"UpdateIPS";


	private:
		/**
		 * \brief A map that associates each chronometer with a unique name.
		 */
		std::map<std::wstring, spk::Chronometer> _chronometers;

		/**
		 * \brief A map that associates each counter with a unique name.
		 */
		std::map<std::wstring, size_t> _counters;

		/**
		 * \brief Private constructor to implement the Singleton pattern.
		 *
		 * This constructor checks whether a TimeMetrics instance has been created.
		 * If not, it throws an error, since Profiler requires TimeMetrics.
		 */
		Profiler();

	public:
		/**
		 * \brief Retrieves the chronometer associated with a specific name.
		 * \param p_key The name of the chronometer.
		 * \throw std::runtime_error if no chronometer exists with the given name.
		 * \return The specified chronometer.
		 */
		const spk::Chronometer &chronometer(const std::wstring &p_key) const;

		/**
		 * \brief Resets the chronometer associated with a specific name.
		 * \param p_key The name of the chronometer.
		 * \throw std::runtime_error if no chronometer exists with the given name.
		 */
		void resetChronometer(const std::wstring &p_key);

		/**
		 * \brief Starts the chronometer associated with a specific name.
		 * \param p_key The name of the chronometer.
		 */
		void startChronometer(const std::wstring &p_key);

		/**
		 * \brief Resumes the chronometer associated with a specific name.
		 * \param p_key The name of the chronometer.
		 * \throw std::runtime_error if no chronometer exists with the given name.
		 */
		void resumeChronometer(const std::wstring &p_key);

		/**
		 * \brief Stops the chronometer associated with a specific name.
		 * \param p_key The name of the chronometer.
		 * \throw std::runtime_error if no chronometer exists with the given name.
		 * \return The time elapsed since the chronometer was started.
		 */
		const long long stopChronometer(const std::wstring &p_key);

		void increseCounter(const std::wstring& p_key);
		void setCounter(const std::wstring& p_key, const size_t& p_value);
		void resetCounter(const std::wstring& p_key);
		const size_t& counter(const std::wstring& p_key) const;
	};
}
