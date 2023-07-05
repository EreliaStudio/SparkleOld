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

		private:

			/**
			 * \brief A map that associates each chronometer with a unique name.
			 */
			std::map<std::wstring, spk::Chronometer> _chronometers;

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
			 * \param p_name The name of the chronometer.
			 * \throw std::runtime_error if no chronometer exists with the given name.
			 * \return The specified chronometer.
			 */
			const spk::Chronometer &chronometer(const std::wstring &p_name) const;

			/**
			 * \brief Resets the chronometer associated with a specific name.
			 * \param p_name The name of the chronometer.
			 * \throw std::runtime_error if no chronometer exists with the given name.
			 */
			void resetChronometer(const std::wstring &p_name);

			/**
			 * \brief Starts the chronometer associated with a specific name.
			 * \param p_name The name of the chronometer.
			 */
			void startChronometer(const std::wstring &p_name);

			/**
			 * \brief Resumes the chronometer associated with a specific name.
			 * \param p_name The name of the chronometer.
			 * \throw std::runtime_error if no chronometer exists with the given name.
			 */
			void resumeChronometer(const std::wstring &p_name);

			/**
			 * \brief Stops the chronometer associated with a specific name.
			 * \param p_name The name of the chronometer.
			 * \throw std::runtime_error if no chronometer exists with the given name.
			 * \return The time elapsed since the chronometer was started.
			 */
			const long long stopChronometer(const std::wstring &p_name);
		};
	}
}


