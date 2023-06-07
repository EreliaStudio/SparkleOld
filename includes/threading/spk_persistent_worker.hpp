#pragma once
#include "threading/spk_thread.hpp"

namespace spk
{
	/**
	 * @brief A thread that will execute a function more than once.
	 *
	 */
	class PersistentWorker : public Thread
	{
	private:
		std::vector<std::function<void()>> _jobs;
		bool _isRunning = false;

	public:
		PersistentWorker() :
			Thread(LaunchMethod::Delayed, L"PersistentWorker", [&]() {
				while (_isRunning)
					for (auto &job : _jobs)
						job();
			}),
			_jobs(),
			_isRunning(false)
		{}

		~PersistentWorker()
		{
			stop();
		}

		void addJob(std::function<void()> p_job)
		{
			_jobs.push_back(p_job);
		}

		void start()
		{
			_isRunning = true;
			Thread::start();
		}

		void stop()
		{
			_isRunning = false;
		}
	};
}