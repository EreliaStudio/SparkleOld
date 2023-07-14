#include "application/spk_abstract_application.hpp"

namespace spk
{
	ContractProvider::Contract AbstractApplication::addJob(const std::wstring &p_WorkerName, const AbstractApplication::Job& p_job)
	{
		if (_workers.find(p_WorkerName) == _workers.end())
			_workers[p_WorkerName] = new spk::PersistentWorker(p_WorkerName);
		return (std::move(_workers[p_WorkerName]->addJob(p_job)));
	}

	ContractProvider::Contract AbstractApplication::addJob(const AbstractApplication::Job& p_job)
	{
		return (std::move(subscribe(_jobs, p_job)));
	}

	AbstractApplication::AbstractApplication() : 
		_errorCode(0),
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

		spk::cout.setPrefix(L"Renderer");
		while (_isRunning)
			for (auto &job : _jobs)
				job();

		for (auto &worker : _workers)
			worker.second->stop();
    
		return _errorCode;
	}

	void AbstractApplication::quit(int p_errorCode)
	{
		_errorCode = p_errorCode;
		_isRunning = false;
	}
}