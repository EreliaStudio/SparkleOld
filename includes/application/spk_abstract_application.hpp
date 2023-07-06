#pragma once

#include <map>
#include <vector>
#include "threading/spk_persistent_worker.hpp"

namespace spk
{
    /**
     * @class AbstractApplication
     * @brief Abstract base class for the application.
     *
     * This class provides a skeletal implementation of an application, leaving the setup of jobs to subclasses.
     */
	class AbstractApplication
	{
	public:
        /**
         * @brief Type alias for a job, represented as a void function without arguments.
         */
		using Job = std::function<void()>;

	private:
        /**
         * @brief Map of persistent worker names to pointers.
         */
		std::map<std::wstring, spk::PersistentWorker *> _workers;
		
        /**
         * @brief Vector of jobs to be performed.
         */
		std::vector<Job> _jobs;

        /**
         * @brief Error code representing the state of the application.
         */
		int _errorCode;
		
        /**
         * @brief Boolean flag indicating if the application is running.
         */
		bool _isRunning;

	protected:
        /**
         * @brief Adds a job to a specific worker.
         *
         * @param p_WorkerName The name of the worker.
         * @param p_job The job to be added.
         */
		void addJob(const std::wstring &p_WorkerName, const Job& p_job);

        /**
         * @brief Adds a job to the application.
         *
         * @param p_job The job to be added.
         */
		void addJob(const Job& p_job);

        /**
         * @brief Virtual function to setup jobs, to be implemented by subclasses.
         */
		virtual void setupJobs() = 0;

	public:
        /**
         * @brief Constructs the AbstractApplication object.
         */
		AbstractApplication();

        /**
         * @brief Destructs the AbstractApplication object.
         */
		~AbstractApplication();

        /**
         * @brief Runs the application.
         *
         * @return The error code after the application finishes running.
         */
		int run();

        /**
         * @brief Quits the application with a specific error code.
         *
         * @param p_errorCode The error code with which the application should quit.
         */
		void quit(int p_errorCode);
	};
}
