#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "threading/spk_thread.hpp"

namespace spk
{
    template<size_t NB_THREAD>
	class WorkerPool
	{
    public:
        using Job = std::function<void()>;

	private:
        spk::Thread* _workers[NB_THREAD];
        spk::ThreadSafeQueue<Job> _jobQueue;
        bool _running = false;
	
	public:
		WorkerPool(std::wstring p_poolName)
        {
            for (size_t i = 0; i < NB_THREAD; i++)
            {
                _workers[i] = new spk::Thread(spk::Thread::LaunchMethod::Delayed, p_poolName + L"/Worker - " + std::to_wstring(i), [&](){
                    while (_running == true)
                    {
                        if (_jobQueue.empty() == false)
                        {
                            Job tmp = _jobQueue.pop_front();
                            tmp();
                        }
                    }
                });
            }
        }

        ~WorkerPool()
        {
            stop();
        }

        void start()
        {
            _running = true;
    
            for (size_t i = 0; i < NB_THREAD; i++)
                _workers[i]->start();
        }

        void stop()
        {
            _running = false;
            for (size_t i = 0; i < NB_THREAD; i++)
                _workers[i]->join();
        }

		template <typename Funct, typename... Args>
		void addJob(Funct&& p_funct, Args&&... p_args)
        {
            _jobQueue.push_back(std::bind(std::forward<Funct>(p_funct), std::forward<Args>(p_args)...));
        }
	};

    using Worker = WorkerPool<1>;
}
