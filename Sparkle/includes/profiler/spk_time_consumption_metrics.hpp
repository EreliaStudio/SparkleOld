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

        bool needEmition() const;

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

    /**
     * @class ScopedTimeConsumptionMetrics
     * @brief A scope-bound utility class for automatic time measurement.
     *
     * ScopedTimeConsumptionMetrics is designed to automatically start and stop time measurement
     * for a given TimeConsumption instance. When an object of this class is created,
     * it starts the timer on the associated TimeConsumption instance. When the object goes out of scope
     * and is destroyed, it stops the timer. This is particularly useful for measuring the duration of
     * a code block or a function's execution.
     */
    class ScopedTimeConsumptionMetrics 
    {
    private:
        TimeConsumption& _target; ///< Reference to the TimeConsumption instance being measured.

    public:
        /**
         * @brief Constructs a ScopedTimeConsumptionMetrics object and starts the timer.
         * @param p_target Reference to a TimeConsumption instance for time measurement.
         */
        ScopedTimeConsumptionMetrics(TimeConsumption& p_target) :
            _target(p_target)
        {
            _target.start();
        }

        /**
         * @brief Destructs the ScopedTimeConsumptionMetrics object and stops the timer.
         */
        ~ScopedTimeConsumptionMetrics()
        {
            _target.stop();
        }
    };
}

#ifndef _DEBUG
#define MONITOR_NAMED_ACTION_TIME_CONSUMPTION(timeMetricName, action){[&](){ action; }();}
#else
#define MONITOR_NAMED_ACTION_TIME_CONSUMPTION(timeMetricName, action)                     \
{                                                                    \
	static spk::TimeConsumption& timeModuleMetric = spk::Application::instance()->profiler().metrics<spk::TimeConsumption>(timeMetricName); \
    spk::ScopedTimeConsumptionMetrics tmpMetric(timeModuleMetric);   \
    [&](){ action; }();                                              \
}
#endif

#ifndef _DEBUG
#define MONITOR_AND_CREATE_TIME_CONSUMPTION(timeMetricName){}
#else
#define MONITOR_AND_CREATE_TIME_CONSUMPTION(timeMetricName)                     \
{                                                                    \
	static spk::TimeConsumption& timeModuleMetric = spk::Application::instance()->profiler().metrics<spk::TimeConsumption>(timeMetricName); \
    spk::ScopedTimeConsumptionMetrics tmpMetric(timeModuleMetric);   \
}
#endif

#ifndef _DEBUG
#define MONITOR_TIME_CONSUMPTION(timeMetrics){}
#else
#define MONITOR_TIME_CONSUMPTION(timeMetrics)                     \
{                                                                 \
    spk::ScopedTimeConsumptionMetrics tmpMetric(timeMetrics);     \
}
#endif