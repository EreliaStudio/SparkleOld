#include "threading/spk_persistent_worker.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	PersistentWorker::PersistentWorker(const std::wstring& p_threadName, const Job& p_job) :
		_isRunning(false),
		Thread(p_threadName, [&, p_job]() {
			_starterSignal.get_future().wait();
			_runJob(p_job);
		})
	{
		
	}

	PersistentWorker::~PersistentWorker()
	{
		stop();
	}

	void PersistentWorker::start()
	{
		_starterSignal.set_value();
	}

	void PersistentWorker::stop()
	{
		_isRunning = false;
		join();
	}

	void PersistentWorker::_runJob(const Job& p_job)
	{
		_isRunning = true;

		while (_isRunning == true)
		{
			p_job();
		}
	}
}