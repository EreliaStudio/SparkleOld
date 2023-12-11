#include "threading/spk_persistent_worker.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	PersistentWorker::PersistentWorker(const std::wstring &p_name) :
		Thread(LaunchMethod::Delayed, p_name, [&]()
		{
			while (_isRunning)
			{
				if (_isPaused == true)
					continue;

				for (const auto &job : _jobs)
				{
					try 
					{
						if (job != nullptr)
							job();
					}
					catch (std::exception& e)
					{
						redirectException(e, _activeJobName);
						_isRunning = false;
					}
				}
			}
		}),
		_jobs(),
		_isRunning(false),
		_isPaused(false)
	{
	}

	PersistentWorker::~PersistentWorker()
	{
		stop();
	}

	std::shared_ptr<ContractProvider::Contract> PersistentWorker::addJob(const std::wstring &p_jobName, const PersistentWorker::Job &p_job)
	{
		std::function<void()> funct = [this, p_jobName, p_job]()
		{
			_activeJobName = &p_jobName;
			p_job();
			_activeJobName = nullptr;
		};
		return (ContractProvider::subscribe(_jobs, funct));
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
			throw;
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