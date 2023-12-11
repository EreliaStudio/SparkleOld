#pragma once

#include <stddef.h>

namespace spk
{
	/**
	 * @class TimeModule
	 * @brief Class for managing time-related functionality.
	 *
	 * This class provides methods to handle and manage time-related functionality within the application.
	 * This class handle the TimeMetrics singleton.
	 */
	class TimeModule
	{
	private:
		size_t _msPerFrame;
	public:
		/**
		 * @brief Constructs the TimeModule object.
		 * Will instanciate the TimeMetrics
		 */
		TimeModule();

		/**
		 * @brief Destructs the TimeModule object.
		 * Will release the TimeMetrics instance
		 */
		~TimeModule();

		/**
		 * @brief Updates the time state.
		 * Will update the TimeMetrics
		 */
		void updateTimeMetrics();

		/**
		 * @brief Set the maximum reachable FPS
		*/
		void setMaxFPS(const size_t& p_nbFPSPerSecond);

		/**
		 * @brief Execute a sleep to reach the next frame
		*/
		void wait();
	};
}
