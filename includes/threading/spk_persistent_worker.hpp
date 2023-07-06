#pragma once

#include "threading/spk_thread.hpp"
#include "design_pattern/spk_contract_provider.hpp"

namespace spk
{
    /**
     * @class PersistentWorker
     * @brief Class for creating and managing a persistent worker thread.
     * 
     * This class extends the Thread class and provides additional functionality 
     * for adding jobs to the worker and controlling its execution.
     */
	class PersistentWorker : public Thread, public ContractProvider
	{
	public:
        /**
         * @typedef Job
         * @brief The type of job to be executed by the worker.
         * 
         * This typedef defines the type of job to be executed by the worker.
         */
		using Job = ContractProvider::Callback;

	private:
		CallbackContainer _jobs;
		std::deque<Contract> _contracts;

		bool _isRunning = false;
		bool _isPaused = false;

	public:
        /**
         * @brief Construct a new PersistentWorker object with a specified name.
         * 
         * This constructor creates a new PersistentWorker object with the specified name.
         * 
         * @param p_name The name of the worker thread.
         */
		PersistentWorker(const std::wstring & p_name);

        /**
         * @brief Destroy the PersistentWorker object.
         * 
         * This is the destructor for the PersistentWorker class.
         */
		~PersistentWorker();

        /**
         * @brief Add a job to the worker.
         * 
         * This function adds a job to the worker. The job will be executed by the worker thread.
         * 
         * @param p_job The job to be added.
         * @return A contract representing the job.
         */
		Contract& addJob(const Job& p_job);

        /**
         * @brief Check if the worker is running.
         * 
         * This function checks if the worker is currently running.
         * 
         * @return True if the worker is running, false otherwise.
         */
		constexpr bool isRunning() const { return (_isRunning); }

        /**
         * @brief Start the worker.
         * 
         * This function starts the worker thread.
         */
		void start();

        /**
         * @brief Stop the worker.
         * 
         * This function stops the worker thread.
         */
		void stop();

        /**
         * @brief Join the worker.
         * 
         * This function is deleted and not available in the PersistentWorker class.
         */
		void join() = delete;

        /**
         * @brief Pause the worker.
         * 
         * This function pauses the execution of the worker thread.
         */
		void pause();

        /**
         * @brief Resume the worker.
         * 
         * This function resumes the execution of the worker thread.
         */
		void resume();
	};
}
