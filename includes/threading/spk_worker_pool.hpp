#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "threading/spk_thread.hpp"

namespace spk
{
	/**
	 * @class WorkerPool
	 * @brief Class for creating and managing a pool of worker threads.
	 * 
	 * This class provides a pool of worker threads that can execute jobs concurrently.
	 */
	class WorkerPool
	{
	public:
		/**
		 * @typedef Job
		 * @brief The type of job to be executed by the workers.
		 * 
		 * This typedef defines the type of job to be executed by the workers.
		 */
		using Job = std::function<void()>;

	private:
		std::vector<spk::Thread*> _workers;
		spk::ThreadSafeQueue<Job> _jobQueue;
		bool _running = false;
	
	public:
		/**
		 * @brief Construct a new WorkerPool object with a specified pool name and number of workers.
		 * 
		 * This constructor creates a new WorkerPool object with the specified pool name and number of workers.
		 * 
		 * @param p_poolName The name of the worker pool.
		 * @param p_nbWorker The number of workers in the pool.
		 */
		WorkerPool(const std::wstring& p_poolName, const size_t& p_nbWorker);

		/**
		 * @brief Copy constructor is deleted.
		 * 
		 * Copying a WorkerPool is not allowed to avoid potential issues with the shared state.
		 */
		WorkerPool(const WorkerPool& p_other) = delete;

		/**
		 * @brief Destroy the WorkerPool object.
		 * 
		 * This is the destructor for the WorkerPool class. It stops the worker pool and cleans up resources.
		 */
		~WorkerPool();

		/**
		 * @brief Start the worker pool.
		 * 
		 * This function starts the worker pool by starting all the worker threads.
		 */
		void start();

		/**
		 * @brief Stop the worker pool.
		 * 
		 * This function stops the worker pool by stopping all the worker threads and clearing the job queue.
		 */
		void stop();

		/**
		 * @brief Add a job to the worker pool.
		 * 
		 * This function adds a job to the worker pool. The job will be executed by one of the worker threads.
		 * 
		 * @tparam Funct The type of the job function.
		 * @tparam Args The types of the arguments of the job function.
		 * @param p_funct The job function.
		 * @param p_args The arguments of the job function.
		 */
		template <typename Funct, typename... Args>
		void addJob(Funct&& p_funct, Args&&... p_args)
		{
			_jobQueue.push_back(std::bind(std::forward<Funct>(p_funct), std::forward<Args>(p_args)...));
		}
	};

	/**
	 * @class Worker
	 * @brief Class for creating a worker thread.
	 * 
	 * This class extends the WorkerPool class and provides a convenient way to create a single worker thread.
	 */
	class Worker : public spk::WorkerPool
	{
	private:

	public:
		/**
		 * @brief Construct a new Worker object with a specified name.
		 * 
		 * This constructor creates a new Worker object with the specified name.
		 * 
		 * @param p_name The name of the worker thread.
		 */
		Worker(const std::wstring& p_name);
	};
}
