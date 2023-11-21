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
	class AbstractApplication : public spk::ContractProvider, public std::enable_shared_from_this<AbstractApplication>
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
		std::map<std::wstring, std::unique_ptr<spk::PersistentWorker>> _workers;
		
		/**
		 * @brief Vector of jobs to be performed.
		 */
		CallbackContainer _jobs;
		const std::wstring* _activeJobName = nullptr;

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
		 * @param p_jobName The name of the new job to be added.
		 * @param p_job The job to be added.
		 */
		Contract addJob(const std::wstring &p_WorkerName, const std::wstring &p_jobName, const Job& p_job);

		/**
		 * @brief Adds a job to the application.
		 *
		 * @param p_jobName The name of the new job to be added.
		 * @param p_job The job to be added.
		 */
		Contract addJob(const std::wstring &p_jobName, const Job& p_job);

		/**
		 * @brief Virtual function to setup jobs, to be implemented by subclasses.
		 */
		virtual void setupJobs() = 0;

		void _renameThread(const std::wstring& p_threadName);

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
