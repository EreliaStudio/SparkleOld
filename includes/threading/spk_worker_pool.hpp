#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "threading/spk_thread.hpp"

namespace spk
{
	class WorkerPool
	{
    public:
        using Job = std::function<void()>;

	private:
        std::vector<spk::Thread*> _workers;
        spk::ThreadSafeQueue<Job> _jobQueue;
        bool _running = false;
	
	public:
		WorkerPool(const std::wstring& p_poolName, const size_t& p_nbWorker);
        WorkerPool(const WorkerPool& p_other) = delete;

        ~WorkerPool();

        void start();
        void stop();

		template <typename Funct, typename... Args>
		void addJob(Funct&& p_funct, Args&&... p_args)
        {
            _jobQueue.push_back(std::bind(std::forward<Funct>(p_funct), std::forward<Args>(p_args)...));
        }
	};

    class Worker : public spk::WorkerPool
    {
	private:

	public:
		Worker(const std::wstring& p_name);
    };
}
