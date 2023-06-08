#include "application/spk_abstract_application.hpp"

namespace spk
{
	void AbstractApplication::addJob(const std::wstring &p_WorkerName, std::function<void()> p_Job)
	{
		if (_workers.find(p_WorkerName) == _workers.end())
			_workers[p_WorkerName] = new spk::PersistentWorker(p_WorkerName);
		_workers[p_WorkerName]->addJob(p_Job);
	}

	void AbstractApplication::addJob(std::function<void()> p_Job)
	{
		_jobs.push_back(p_Job);
	}

	AbstractApplication::AbstractApplication() : _errorCode(0),
							_isRunning(false)
	{
	}

	AbstractApplication::~AbstractApplication()
	{
		_isRunning = false;

		for (auto &worker : _workers)
			delete worker.second;
	}

	int AbstractApplication::run()
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

	void AbstractApplication::quit(int p_errorCode)
	{
		_errorCode = p_errorCode;
		_isRunning = false;

		for (auto &worker : _workers)
			worker.second->stop();
	}
}