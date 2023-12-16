#pragma once

#include "threading/spk_thread.hpp"
#include <future>

namespace spk
{
	/**
     * @brief Class representing a persistent worker that executes a specific job in a dedicated thread.
     *
     * The PersistentWorker class is designed to run a given job in a separate thread
     * continuously until instructed to stop. It provides functionality to start and stop
     * the execution of the job.
     */
    class PersistentWorker : public Thread
    {
    public:
        using Job = std::function<void()>; ///< Alias for the job function type.

    private:
        bool _isRunning; ///< Flag to indicate if the worker is currently running.
        std::promise<void> _starterSignal; ///< Signal used to control the start of the job.

        /**
         * @brief Internal method to run the given job.
         * @param p_job The job to be executed by this worker.
         */
        void _runJob(const Job& p_job);

    public:
        /**
         * @brief Constructs a PersistentWorker with a specified job.
         * @param p_threadName The name of the worker thread.
         * @param p_job The job to be executed by this worker.
         */
        PersistentWorker(const std::wstring& p_threadName, const Job& p_job);

        /**
         * @brief Destructs the PersistentWorker instance.
         */
        ~PersistentWorker();

        /**
         * @brief Starts the execution of the job in a separate thread.
         */
        void start();

        /**
         * @brief Stops the execution of the job.
         */
        void stop();
    };
}
