#pragma once

#include "threading/spk_thread.hpp"
#include "threading/spk_thread_safe_queue.hpp"

namespace spk
{
    /**
     * @class Worker
     * @brief A thread class specialized in executing a queue of jobs.
     *
     * Worker extends the Thread class to process a queue of jobs. Jobs can be any function
     * and are stored in a thread-safe queue. This class is suitable for tasks that require
     * asynchronous processing or background task execution.
     */
    class Worker : public Thread
    {
    private:
        spk::ThreadSafeQueue<std::function<void()>> _jobQueue; ///< Queue of jobs to be executed by the worker.
        bool _isRunning; ///< Indicates whether the worker is currently running.

    public:
        /**
         * @brief Constructs a Worker with a specified name.
         * @param p_name The name of the worker thread.
         */
        Worker(const std::wstring& p_name);

        /**
         * @brief Destructor for the Worker class.
         *
         * Ensures that all pending jobs are processed or properly handled before destruction.
         */
        ~Worker();

        /**
         * @brief Adds a new job to the worker's job queue.
         * 
         * This template function allows adding jobs with various signatures to the queue.
         * 
         * @tparam Func Type of the function representing the job.
         * @tparam Args Variadic template for arguments to the function.
         * @param func The function representing the job.
         * @param args Arguments to be passed to the function.
         */
        template <typename Func, typename... Args>
        void addJob(Func&& func, Args&&... args)
		{
            auto boundTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
            _jobQueue.push_back(boundTask);
        }

        /**
         * @brief Signals the worker to stop processing and prepares it for shutdown.
         */
        void stop();
    };
}
