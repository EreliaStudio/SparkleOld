#include "threading/spk_worker_pool.hpp"

namespace spk
{
    WorkerPool::WorkerPool(const std::wstring& p_poolName, const size_t& p_nbWorker)
    {
        for (size_t i = 0; i < p_nbWorker; i++)
        {
            std::wstring poolName = p_poolName;

            if (p_nbWorker > 1)
                poolName += L"/Worker - " + std::to_wstring(i);
                
            _workers.push_back(new spk::Thread(spk::Thread::LaunchMethod::Delayed, poolName, [&](){
                while (_running == true)
                {
                    if (_jobQueue.empty() == false)
                    {
                        Job tmp = _jobQueue.pop_front();
                        tmp();
                    }
                }
            }));
        }
    }

    WorkerPool::~WorkerPool()
    {
        stop();
    }

    void WorkerPool::start()
    {
        _running = true;

        for (size_t i = 0; i < _workers.size(); i++)
            _workers[i]->start();
    }

    void WorkerPool::stop()
    {
        _running = false;
        for (size_t i = 0; i < _workers.size(); i++)
            _workers[i]->join();
    }

    Worker::Worker(const std::wstring& p_name) :
        spk::WorkerPool(p_name, 1)
    {

    }
}
