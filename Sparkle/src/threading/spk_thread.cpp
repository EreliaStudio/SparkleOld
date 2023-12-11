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
			_starterSignal.set_value();
			while (_isActive == false);
		}
		catch(const std::exception&)
		{
			throw std::runtime_error("Trying to restart an already launched thread");
		}
	}
}
