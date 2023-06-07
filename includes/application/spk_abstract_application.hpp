#pragma once

#include <map>
#include <vector>
#include <string>
#include <functional>
#include "threading/spk_persistent_worker.hpp"

namespace spk
{
	class AbstractApplication
	{
	private:
		std::map<std::wstring, spk::PersistentWorker *> _workers;
		std::vector<std::function<void()>> _jobs;

		int _errorCode;
		bool _isRunning;

	protected:
		void addJob(const std::wstring &p_WorkerName, std::function<void()> p_Job)
		{
			if (_workers.find(p_WorkerName) == _workers.end())
				_workers[p_WorkerName] = new spk::PersistentWorker(p_WorkerName);
			_workers[p_WorkerName]->addJob(p_Job);
		}

		void addJob(std::function<void()> p_Job)
		{
			_jobs.push_back(p_Job);
		}

		virtual void setupJobs() = 0;

	public:
		AbstractApplication() : _errorCode(0),
								_isRunning(false)
		{
		}

		~AbstractApplication()
		{
			_isRunning = false;

			for (auto &worker : _workers)
				delete worker.second;
		}

		int run()
		{
			setupJobs();
			_errorCode = 0;
			_isRunning = true;

			for (auto &worker : _workers)
				worker.second->start();
			while (_isRunning)
				for (auto &job : _jobs)
					job();

			return _errorCode;
		}

		void quit(int p_errorCode)
		{
			_errorCode = p_errorCode;
			_isRunning = false;

			for (auto &worker : _workers)
				worker.second->stop();
		}
	};
}