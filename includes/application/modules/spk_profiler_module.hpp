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
		float _counterRatio;

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
		 * @brief Actualize the affiched FPS/UPS counter
		 */
		void updateCounters();

		/**
		 * @brief Updating FPS counter by incrementing it by one
		*/
		void updateFPS();

		/**
		 * @brief Updating UPS counter by incrementing it by one
		*/
		void updateUPS();
	};
}
