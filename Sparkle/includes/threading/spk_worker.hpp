#pragma once

#include "threading/spk_thread.hpp"
#include "threading/spk_thread_safe_queue.hpp"

namespace spk
{
    class Worker : public Thread
    {
    private:
        spk::ThreadSafeQueue<std::function<void()>> _jobQueue;
        bool _isRunning;

    public:
        Worker(const std::wstring& p_name);
        ~Worker();

        template <typename Func, typename... Args>
        void addJob(Func&& func, Args&&... args)
		{
            auto boundTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
            _jobQueue.push_back(boundTask);
        }

        void stop();
    };
}
