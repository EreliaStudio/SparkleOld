#pragma once

#include "profiler/spk_imetrics.hpp"
#include "system/spk_chronometer.hpp"

namespace spk
{
    /**
     * @class TimeConsumption
     * @brief A class for tracking and reporting time consumption metrics.
     *
     * TimeConsumption extends the IMetrics class to specifically track and report on time-related metrics.
     * It uses an internal Chronometer to measure elapsed time and provides functionalities to start and stop the timer.
     * Additionally, it calculates CPU usage and total duration, offering a detailed view of time consumption.
     */
    class TimeConsumption : public IMetrics<long long>
    {
    private:
        spk::Chronometer _chrono; ///< Chronometer used to measure time intervals.
        long long _totalDuration; ///< Total duration measured by this instance.

    public:
        /**
         * @brief Constructs a TimeConsumption instance.
         */
        TimeConsumption();

        /**
         * @brief Creates a JSON object containing the time consumption metrics report.
         * @return JSON::Object representing the metrics.
         */
        spk::JSON::Object emitReport() const override;

        /**
         * @brief Starts the chronometer to begin measuring time.
         */
        void start();

        /**
         * @brief Stops the chronometer and adds the elapsed time to total duration.
         */
        void stop();

        /**
         * @brief Calculates and returns the CPU usage percentage.
         * @return Double representing the CPU usage.
         */
        double CPUUsage() const;

        /**
         * @brief Accessor for the total duration measured.
         * @return Constant reference to the total duration.
         */
        const long long& totalDuration() const;
    };
}