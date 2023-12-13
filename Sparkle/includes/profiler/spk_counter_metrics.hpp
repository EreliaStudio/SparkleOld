#pragma once

#include "profiler/spk_imetrics.hpp"

namespace spk
{
    /**
     * @brief A class that extends IMetrics for size_t type, designed to count and track values.
     *
     * Counter provides functionality to increment, save, and report metrics. It also
     * supports standard arithmetic operations.
     */
    class Counter : public IMetrics<size_t>
    {
    private:
        size_t _value; ///< Internal counter value.

    public:
        /**
         * @brief Constructor for Counter.
         */
        Counter();

        /**
         * @brief Creates a JSON object containing the metrics report.
         * @return JSON::Object representing the metrics.
         */
        spk::JSON::Object emitReport() const override;

        /**
         * @brief Saves the current state of the counter to the metrics.
         */
        void save();

        /**
         * @brief Increments the counter by a given delta.
         * @param p_delta The value to increment the counter by.
         */
        void increment(const size_t& p_delta);

        /**
         * @brief Overloads the += operator to add a value to the counter.
         * @param p_delta The value to add to the counter.
         * @return Reference to the current Counter object.
         */
        Counter& operator+=(const size_t& p_delta);

        /**
         * @brief Prefix increment operator.
         * @return Reference to the incremented Counter object.
         */
        Counter& operator++();

        /**
         * @brief Postfix increment operator.
         * @return Counter object before the increment.
         */
        Counter operator++(int);
    };
}
