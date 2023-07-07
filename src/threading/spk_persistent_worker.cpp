#include "threading/spk_persistent_worker.hpp"

namespace spk
{
	PersistentWorker::PersistentWorker(const std::wstring & p_name) :
		Thread(LaunchMethod::Delayed, p_name, [&]() {
			while (_isRunning)
			{
				if (_isPaused == true)
					continue;
				for (auto &job : _jobs)
				{
					try 
					{
						job();
					}
					catch (...)
					{

					}
				}
			}
		}),
		_jobs(),
		_isRunning(false),
		_isPaused(false)
	{}

	PersistentWorker::~PersistentWorker()
	{
		stop();
	}

	ContractProvider::Contract PersistentWorker::addJob(const PersistentWorker::Job& p_job)
	{
		return (std::move(ContractProvider::subscribe(_jobs, p_job)));
	}

	/**
	 * @brief Start the thread that handle the jobs.
	 * 
	 * @warning start will throw an exception if the thread is already running
	 */
	void PersistentWorker::start()
	{
		_isRunning = true;
		_isPaused = false;
		try
		{
			Thread::start();
		}
		catch (...)
		{
			throw ;
		}
	}

	void PersistentWorker::stop()
	{
		_isRunning = false;
		Thread::join();
	}

	void PersistentWorker::pause()
	{
		_isPaused = true;
	}

	void PersistentWorker::resume()
	{
		_isPaused = false;
	}
}