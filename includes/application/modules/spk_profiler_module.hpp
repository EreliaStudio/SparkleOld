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
		/**
		 * \brief Timer that ticks every second.
		 */
		spk::Timer _oneSecondTime;

		/**
		 * \brief Counter for the number of frames rendered per second.
		 */
		size_t _renderIPSCounter;

		/**
		 * \brief Counter for the number of updates processed per second.
		 */
		size_t _updateIPSCounter;

		/**
		 * \brief Resets the counters for frames rendered and updates processed.
		 */
		void _resetCounter();

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
		 * \brief Update the counters and timer.
		 *
		 * This method should be called once per frame in the application's main update loop.
		 */
		void updateData();

		/**
		 * \brief Increment the counter for frames rendered per second.
		 *
		 * This method should be called once per frame rendered.
		 */
		void increaseRenderIPS();

		/**
		 * \brief Increment the counter for updates processed per second.
		 *
		 * This method should be called once per update processed.
		 */
		void increaseUpdateIPS();
	};
}
