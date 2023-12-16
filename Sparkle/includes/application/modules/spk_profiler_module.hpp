#pragma once

#include "profiler/spk_profiler.hpp"

namespace spk
{
    /**
     * @brief A module class that encapsulates profiling functionality.
     *
     * ProfilerModule serves as a wrapper around the Profiler class, providing
     * an interface for profiling operations within the application. It includes
     * functionalities to manage and emit reports from the encapsulated Profiler.
     */
    class ProfilerModule
    {
    private:
        spk::Profiler _profiler; ///< Instance of the Profiler class.

    public:
        /**
         * @brief Constructs a ProfilerModule instance.
         */
        ProfilerModule();

        /**
         * @brief Destructs the ProfilerModule instance.
         */
        ~ProfilerModule();

        /**
         * @brief Emits a profiling report.
         *
         * This function collects and emits the profiling data gathered by the Profiler.
         */
        void emitReport();

        /**
         * @brief Provides non-const access to the encapsulated Profiler.
         * @return Reference to the Profiler instance.
         */
        spk::Profiler& profiler() { return _profiler; }

        /**
         * @brief Provides const access to the encapsulated Profiler.
         * @return Const reference to the Profiler instance.
         */
        const spk::Profiler& profiler() const { return _profiler; }
    };
}

