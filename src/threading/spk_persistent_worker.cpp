#include "threading/spk_persistent_worker.hpp"

namespace spk
{
	PersistentWorker::PersistentWorker(const std::wstring & p_name) :
		Thread(LaunchMethod::Delayed, p_name, [&]() {
			while (_isRunning)
				for (auto &job : _jobs)
					job();
		}),
		_jobs(),
		_isRunning(false)
	{}

	PersistentWorker::~PersistentWorker()
	{
		stop();
	}

	void PersistentWorker::addJob(const PersistentWorker::Job& p_job)
	{
		_jobs.push_back(p_job);
	}

	void PersistentWorker::start()
	{
		_isRunning = true;
		Thread::start();
	}

	void PersistentWorker::stop()
	{
		_isRunning = false;
	}
}