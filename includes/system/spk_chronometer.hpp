#pragma once

#include "system/spk_time_metrics.hpp"
#include "threading/spk_persistent_worker.hpp"

namespace spk
{
    /**
     * @class Chronometer
     * @brief A simple chronometer class to measure elapsed time.
     */
    class Chronometer
    {
    private:
        long long _start; ///< Start time.
        mutable long long _duration; ///< Current duration.
        mutable long long _totalDuration; ///< Total duration.
        bool _isRunning; ///< True if chronometer is running, false otherwise.

    public:
        /**
         * @brief Default constructor.
         */
        Chronometer();

        /**
         * @brief Destructor.
         */
        ~Chronometer();

        /**
         * @brief Start the chronometer.
         * @throws std::runtime_error if the chronometer is already running.
         */
        void start();

        /**
         * @brief Resume the chronometer from the time it was stopped.
         * @throws std::runtime_error if the chronometer is already running.
         */
        void resume();

        /**
         * @brief Check if the chronometer is currently running.
         * @return True if the chronometer is running, false otherwise.
         */
        bool isRunning() const;

        /**
         * @brief Get the duration since the start of the chronometer. It take count of previous paused and resumed measurements.
         * @return The duration in milliseconds. If the chronometer is not running, the duration remains the same.
         */
        const long long duration() const;

        /**
		 * @brief Stop the chronometer. Set the status of the chronometer to stopped.
		 * @throws std::runtime_error if the chronometer is already stopped.
		 * @return The total duration in milliseconds. If the chronometer is already stopped, a runtime_error is thrown.
		 */
        const long long& stop();
    };
}
