#include "threading/spk_thread.hpp"

namespace spk
{
    Thread::~Thread()
    {
        join();
    }

    void Thread::join()
    {
        if (_thread.joinable())
            _thread.join();
    }

    void Thread::start()
    {
        try
        {
            _starterSignal.set_value(true);
        }
        catch(const std::exception& e)
        {
            throw std::runtime_error("Trying to restart an already launched thread");
        }
        
    }
}
