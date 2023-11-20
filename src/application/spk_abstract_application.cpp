#include "application/spk_abstract_application.hpp"
#include "spk_basic_functions.hpp"
#include "debug/spk_profiler.hpp"

namespace spk
{
	ContractProvider::Contract AbstractApplication::addJob(const std::wstring& p_WorkerName, const std::wstring& p_jobName, const AbstractApplication::Job& p_job)
	{
		if (_workers.find(p_WorkerName) == _workers.end())
			_workers[p_WorkerName] = std::make_unique<spk::PersistentWorker>(p_WorkerName);
		return (std::move(_workers[p_WorkerName]->addJob(p_jobName, p_job)));
	}

	ContractProvider::Contract AbstractApplication::addJob(const std::wstring& p_jobName, const AbstractApplication::Job& p_job)
	{
		std::function<void()> funct = [this, p_jobName, p_job]() {
			_activeJobName = &p_jobName;
			spk::Profiler::instance()->startChronometer(p_jobName);
			p_job();
			spk::Profiler::instance()->stopChronometer(p_jobName);
			_activeJobName = nullptr;
			};
		return (std::move(ContractProvider::subscribe(_jobs, funct)));
	}

	AbstractApplication::AbstractApplication() :
		_errorCode(0),
		_isRunning(false)
	{
	}

	AbstractApplication::~AbstractApplication()
	{
		_isRunning = false;
	}

	int AbstractApplication::run()
	{
		setupJobs();
		_errorCode = 0;
		_isRunning = true;

		for (auto& worker : _workers)
			worker.second->start();

		spk::cout.setPrefix(L"Renderer");
		while (_isRunning)
		{
			for (auto& worker : _workers)
				if (worker.second->isRunning() == false)
				{
					quit(1);
				}

			for (auto& job : _jobs)
			{
				try
				{
					if (job != nullptr)
						job();
				}
				catch (std::exception& e)
				{
					redirectException(e, _activeJobName);
					quit(1);
				}
			}
		}

		for (auto& worker : _workers)
			worker.second->stop();

		spk::cout.setPrefix(L"");

		return _errorCode;
	}

	void AbstractApplication::quit(int p_errorCode)
	{
		_errorCode = p_errorCode;
		_isRunning = false;
	}
}