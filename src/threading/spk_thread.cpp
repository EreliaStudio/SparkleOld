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
        _starterSignal.set_value(true);
    }
}
