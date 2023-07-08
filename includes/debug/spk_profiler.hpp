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
		static inline const std::wstring RENDER_IPS_COUNTER = L"RenderIPS"; ///< The key representing the counter to the Render thread Iteration Per Second
		static inline const std::wstring UPDATE_IPS_COUNTER = L"UpdateIPS"; ///< The key representing the counter to the Update thread Iteration Per Second


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

		/**
		 * \brief Create a new counter, named p_key
		 * 
		 * If the counter already exist, this method will throw an exception
		 * 
		 * \param p_key The name of the counter.
		 * \throw std::runtime_error if a chronometer exists with the given name.
		*/
		void createCounter(const std::wstring& p_key);

		/**
		 * \brief Increases the value of the specified counter by one.
		 *
		 * If the counter does not exist, it is created with a value of 0, and then incremented.
		 *
		 * \param p_key The name of the counter.
		 */
		void increaseCounter(const std::wstring& p_key);

		/**
		 * \brief Sets the value of the specified counter.
		 *
		 * If the counter does not exist, it is created and then set to the specified value.
		 *
		 * \param p_key The name of the counter.
		 * \param p_value The new value for the counter.
		 */
		void setCounter(const std::wstring& p_key, const size_t& p_value);

		/**
		 * \brief Resets the specified counter to 0.
		 *
		 * If the counter does not exist, a runtime error is thrown.
		 *
		 * \param p_key The name of the counter.
		 * \throw std::runtime_error if no chronometer exists with the given name.
		 */
		void resetCounter(const std::wstring& p_key);

		/**
		 * Check if there is a counter name p_key inside the profiler.
		 * 
		 * \param p_key The name of the counter.
		 * \return True if the counter exist, false otherwise.
		*/
		bool containCounter(const std::wstring& p_key) const;

		/**
		 * \brief Retrieves the value of the specified counter.
		 *
		 * If the counter does not exist, a runtime error is thrown.
		 *
		 * \param p_key The name of the counter.
		 * \return The value of the counter.
		 * \throw std::runtime_error if no chronometer exists with the given name.
		 */
		const size_t& counter(const std::wstring& p_key) const;
	};
}
