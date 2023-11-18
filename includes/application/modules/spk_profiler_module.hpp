#pragma once

#include "system/spk_timer.hpp"

namespace spk
{
	/**
	 * \class ProfilerModule
	 * \brief A class to profile and manage the frames-per-second rates of the application.
	 *
	 * This class is responsible for tracking the number of frames and updates that the application
	 * processes every second (known as the "render IPS" and "update IPS" respectively).
	 */
	class ProfilerModule
	{
	private:
		spk::Timer _counterTimer;

	public:
		/**
		 * \brief Default constructor for the ProfilerModule class.
		 */
		ProfilerModule();

		/**
		 * \brief Destructor for the ProfilerModule class.
		 */
		~ProfilerModule();

		/**
		 * \brief Update the counters.
		 *
		 * This method should be called once per frame in the application's main update loop.
		 */
		void updateCounters();

		void updateFPS();
		void updateUPS();
	};
}
