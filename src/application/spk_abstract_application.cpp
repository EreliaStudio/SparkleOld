#include "application/spk_abstract_application.hpp"
#include "spk_basic_functions.hpp"
#include "debug/spk_profiler.hpp"

namespace spk
{
	std::shared_ptr<ContractProvider::Contract> AbstractApplication::addJob(const std::wstring& p_WorkerName, const std::wstring& p_jobName, const AbstractApplication::Job& p_job)
	{
		if (_workers.find(p_WorkerName) == _workers.end())
			_workers[p_WorkerName] = std::make_unique<spk::PersistentWorker>(p_WorkerName);
		return (std::move(_workers[p_WorkerName]->addJob(p_jobName, p_job)));
	}

	std::shared_ptr<ContractProvider::Contract> AbstractApplication::addJob(const std::wstring& p_jobName, const AbstractApplication::Job& p_job)
	{
		std::function<void()> funct = [this, p_jobName, p_job]() {
				_activeJobName = &p_jobName;
				p_job();
				_activeJobName = nullptr;
			};
		return (ContractProvider::subscribe(_jobs, funct));
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

	void AbstractApplication::_renameThread(const std::wstring& p_threadName)
	{
		spk::cout.setPrefix(p_threadName);
		spk::Profiler::instance()->defineThreadName(p_threadName);
	}
		
	bool AbstractApplication::isRunning() const
	{
		return (_isRunning);
	}

	int AbstractApplication::run()
	{
		setupJobs();
		_errorCode = 0;
		_isRunning = true;

		for (auto& worker : _workers)
			worker.second->start();

		_renameThread(L"Renderer");

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